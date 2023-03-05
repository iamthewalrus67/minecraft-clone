#include "glm/fwd.hpp"
#include <iostream>
#include <stdio.h>

#include <GLFW/glfw3.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/bx.h>
#include <bx/math.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

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

#include "render/camera.hpp"

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

int main(int argc, char **argv) {
    // Logger init
    Logger::instance().init();
    Logger::setDebugMode(true);

    WindowWrapper window{"minesraft", 1024, 768};
    Keyboard &keyboard = Keyboard::instance();
    Mouse &mouse = Mouse::instance();

    // Call bgfx::renderFrame before bgfx::init to signal to bgfx not to
    // create a render thread. Most graphics APIs must be used on the same
    // thread that created the window.
    bgfx::renderFrame();
    // Initialize bgfx using the native window handle and window resolution.
    bgfx::Init init;

    init.platformData.nwh = window.getWindow();

    int width, height;
    window.getSize(&width, &height);
    init.resolution.width = (uint32_t)width;
    init.resolution.height = (uint32_t)height;
    init.resolution.reset = BGFX_RESET_VSYNC;
    init.type = bgfx::RendererType::OpenGL;

    if (!bgfx::init(init))
        return 1;

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

    // Camera init
    rend::Camera cam{60.0f,
                     (int)window.getSize().first,
                     (int)window.getSize().second,
                     {0.0f, 0.0f, 10.0f}};

    size_t counter = 0;

    while (!window.shouldClose()) {
        window.pollEvents();
        // Handle window resize.
        int oldWidth = width, oldHeight = height;
        window.getSize(&width, &height);
        if (width != oldWidth || height != oldHeight) {
            bgfx::reset((uint32_t)width, (uint32_t)height, BGFX_RESET_VSYNC);
            bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);
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

        std::cout << glm::to_string(cam.getViewMatrix()) << std::endl;

        cam.addRotation(
            glm::vec3(glm::radians(0.0f), glm::radians(0.0f), 0.0f));

        bgfx::setViewTransform(0, glm::value_ptr(cam.getViewMatrix()),
                               glm::value_ptr(cam.getProjectionMatrix()));

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

    bgfx::shutdown();
    window.terminate();

    return 0;
}
