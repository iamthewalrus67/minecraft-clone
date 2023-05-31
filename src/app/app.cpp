#include "glm/fwd.hpp"
#include <iostream>

#include <GLFW/glfw3.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/bx.h>
#include <bx/math.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#if BX_PLATFORM_LINUX
#define GLFW_EXPOSE_NATIVE_X11
#elif BX_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#elif BX_PLATFORM_OSX
#define GLFW_EXPOSE_NATIVE_COCOA
#endif

#include "input/keyboard.hpp"
#include "input/mouse.hpp"
#include "logging/logger/logger.hpp"
#include "shader_loading/shader_loading.hpp"
#include "window/window.hpp"

#include "controllers/camera/flying_camera_controller.hpp"
#include "app/app.hpp"

struct PosColorVertex {
    float m_x;
    float m_y;
    float m_z;
    uint32_t m_abgr;

    static void init() {
        ms_decl.begin()
                .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
                .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
                .end();
    };

    static bgfx::VertexLayout ms_decl;
};

bgfx::VertexLayout PosColorVertex::ms_decl;

static PosColorVertex s_cubeVertices[] = {
        {-1.0f, 1.0f, 1.0f, 0xff000000},   {1.0f, 1.0f, 1.0f, 0xff0000ff},
        {-1.0f, -1.0f, 1.0f, 0xff00ff00},  {1.0f, -1.0f, 1.0f, 0xff00ffff},
        {-1.0f, 1.0f, -1.0f, 0xffff0000},  {1.0f, 1.0f, -1.0f, 0xffff00ff},
        {-1.0f, -1.0f, -1.0f, 0xffffff00}, {1.0f, -1.0f, -1.0f, 0xffffffff},
};

static const uint16_t s_cubeTriList[] = {
        1, 0, 2, 3, 1, 2, 6, 4, 5, 6, 5, 7, 2, 0, 4, 2, 4, 6,
        5, 1, 3, 7, 5, 3, 4, 0, 1, 5, 4, 1, 3, 2, 6, 3, 6, 7,
};

bgfx::VertexBufferHandle m_vbh;
bgfx::IndexBufferHandle m_ibh;
bgfx::ProgramHandle m_program;

bool s_showStats = false;

App::App(const char *title, uint32_t width, uint32_t height): m_window(title, width, height) {}

App::~App() {}

void App::init() {

    // Logger init
    Logger::instance().init();
    Logger::setDebugMode(true);

    m_window.init();

    // Call bgfx::renderFrame before bgfx::init to signal to bgfx not to
    // create a render thread. Most graphics APIs must be used on the same
    // thread that created the window.
    bgfx::renderFrame();
    // Initialize bgfx using the native window handle and window resolution.
    bgfx::Init init;

    init.platformData.nwh = m_window.getWindow();

    int width = 1024, height = 768;
    m_window.getSize(&width, &height);
    init.resolution.width = (uint32_t)width;
    init.resolution.height = (uint32_t)height;
    init.resolution.reset = BGFX_RESET_VSYNC;
    init.type = bgfx::RendererType::OpenGL;

    if (!bgfx::init(init)) {
        std::cerr << "Failed to initialize bgfx" << std::endl;
        std::exit(1);
    }
}

void App::start() {
    Keyboard &keyboard = Keyboard::instance();
    Mouse &mouse = Mouse::instance();

    // Cube definition
    PosColorVertex::init();
    m_vbh = bgfx::createVertexBuffer(
        // Static data can be passed with bgfx::makeRef
        bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices)),
        PosColorVertex::ms_decl);

    m_ibh = bgfx::createIndexBuffer(
        // Static data can be passed with bgfx::makeRef
        bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList)));

    bgfx::ShaderHandle vsh = loadShader("res/shaders/vs_simple.bin");
    bgfx::ShaderHandle fsh = loadShader("res/shaders/fs_simple.bin");

    m_program = bgfx::createProgram(vsh, fsh, true);

    // Set view 0 to the same dimensions as the window and to clear the
    // color buffer.
    const bgfx::ViewId kClearView = 0;
    bgfx::setViewClear(kClearView, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x44ff);
    bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);

    int width = 1024, height = 768;
    m_window.getSize(&width, &height);
    // Camera init
    control::FlyingCameraController camController{
        60.0f,
        {width, height},
        {0.0f, 0.0f, 10.0f}};

    size_t counter = 0;
    while (!m_window.shouldClose()) {
        m_window.pollEvents();
        // Handle window resize.
        int oldWidth = width, oldHeight = height;
        m_window.getSize(&width, &height);
        if (width != oldWidth || height != oldHeight) {
            bgfx::reset((uint32_t)width, (uint32_t)height, BGFX_RESET_VSYNC);
            bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);
            camController.updateScreenSize(width, height);
        }

        if (mouse.isLeftButtonJustPressed()) {
            m_window.setCaptureCursor(true);
        }
        if (keyboard.isJustPressed(GLFW_KEY_ESCAPE)) {
            m_window.setCaptureCursor(false);
        }

        // This dummy draw call is here to make sure that view 0 is cleared
        // if no other draw calls are submitted to view 0.
        bgfx::touch(kClearView);
        // Use debug font to print information about this example.
        bgfx::dbgTextClear();
        bgfx::dbgTextPrintf(0, 0, 0x0f, "Press F1 to toggle stats.");
        const bgfx::Stats *stats = bgfx::getStats();
        bgfx::dbgTextPrintf(0, 1, 0x0f,
                            "Backbuffer %dW x %dH in pixels, debug text %dW x "
                            "%dH in characters.",
                            stats->width, stats->height, stats->textWidth,
                            stats->textHeight);
        // Enable stats or debug text.
        if (keyboard.isJustPressed(GLFW_KEY_F1)) {
            s_showStats = !s_showStats;
        }
        bgfx::setDebug(s_showStats ? BGFX_DEBUG_STATS : BGFX_DEBUG_TEXT);

        camController.captureInputAndApply();

        bgfx::setViewTransform(
            0, glm::value_ptr(camController.getCamera().getViewMatrix()),
            glm::value_ptr(camController.getCamera().getProjectionMatrix()));

        // bgfx::setViewRect(kClearView, 0, 0,
        // bgfx::BackbufferRatio::Equal);

        bgfx::touch(0);

        float mtx[16];
        bx::mtxRotateY(mtx, counter * 0.00f);

        // position x,y,z
        mtx[12] = 0.0f;
        mtx[13] = 0.0f;
        mtx[14] = 0.0f;

        // Set model matrix for rendering.
        bgfx::setTransform(mtx);

        // Set vertex and index buffer.
        bgfx::setVertexBuffer(0, m_vbh);
        bgfx::setIndexBuffer(m_ibh);

        // Set render states.
        bgfx::setState(BGFX_STATE_DEFAULT);

        // Submit primitive for rendering to view 0.
        bgfx::submit(0, m_program);

        // Advance to next frame. Process submitted rendering primitives.
        bgfx::frame();

        counter++;
        keyboard.updateKeys();
        mouse.updatePos();
    }
}

void App::terminate() {
    bgfx::shutdown();
    m_window.terminate();
}