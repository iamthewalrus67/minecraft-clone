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

#include "player/player.hpp"

#include "math/ray.hpp"
#include "render/chunk/chunk.hpp"

bool s_showStats = false;

App::App(const char *title, uint32_t width, uint32_t height): m_window(title, width, height) {}

App::~App() {}

void App::init() {

    // Logger init
    Logger::instance().init();
    Logger::setDebugMode(true);

    // Call bgfx::renderFrame before bgfx::init to signal to bgfx not to
    // create a render thread. Most graphics APIs must be used on the same
    // thread that created the window.
    bgfx::renderFrame();
    // Initialize bgfx using the native window handle and window resolution.
    bgfx::Init init;

    m_window.init();

    init.platformData.nwh = m_window.getWindow();

    int width = 1024, height = 768;
    m_window.getSize(&width, &height);
    init.resolution.width = (uint32_t)width;
    init.resolution.height = (uint32_t)height;
    init.resolution.reset = BGFX_RESET_VSYNC;
#if Darvin
    init.type = bgfx::RendererType::Metal;
#else
    init.type = bgfx::RendererType::OpenGL;
#endif
    if (!bgfx::init(init)) {
        std::cerr << "Failed to initialize bgfx" << std::endl;
        std::exit(1);
    }

    initRenderIternal();
}

void App::initRenderIternal() {
    m_renderer.init();

    int width, height;
    m_window.getSize(&width, &height);
    m_cameraController.reset(
            new control::FlyingCameraController{
                60.0f,
                {width, height},
                {0.0f, 0.0f, 10.0f}
            }
    );
}

void App::start() {
    Keyboard &keyboard = Keyboard::instance();
    Mouse &mouse = Mouse::instance();

    // Set view 0 to the same dimensions as the window and to clear the
    // color buffer.
    const bgfx::ViewId kClearView = 0;
    bgfx::setViewClear(kClearView, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x6EB1FF);
    bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);

    int width = 1024, height = 768;
    m_window.getSize(&width, &height);
    m_renderer.setScreenSize(width, height);

    world::WorldManager wm = world::WorldManager();

    player::Player player{m_cameraController, m_renderer.getChunkManagerRef()};

    while (!m_window.shouldClose()) {
        m_window.pollEvents();
        // Handle window resize.
        int oldWidth = width, oldHeight = height;
        m_window.getSize(&width, &height);
        if (width != oldWidth || height != oldHeight) {
            bgfx::reset((uint32_t)width, (uint32_t)height, BGFX_RESET_VSYNC);
            bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);
            m_cameraController->updateScreenSize(width, height);
            m_renderer.setScreenSize(width, height);
        }

        if (mouse.isLeftButtonJustPressed()) {
            m_window.setCaptureCursor(true);
        }
        if (keyboard.isJustPressed(GLFW_KEY_ESCAPE)) {
            m_window.setCaptureCursor(false);
        }

        // EXAMPLE: THIS IS HOW YOU WOULD USE THE REMOVAL!
        // TODO: REMOVE
        if (keyboard.isJustPressed(GLFW_KEY_F)) {
            auto& a = m_renderer.getChunkManagerRef();
            auto cOpt = a.getChunkRefFromGlobalPos(glm::vec3{-10, 0, -10});
            std::cout << cOpt->getChunkGlobalPos().x << " " << cOpt->getChunkGlobalPos().y << " " << cOpt->getChunkGlobalPos().z << std::endl;
            cOpt->setBlock(glm::vec3{15, 0, 0}, 0);
        }

        player.update();

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

        m_cameraController->captureInputAndApply();

        bgfx::setViewTransform(
            0, glm::value_ptr(m_cameraController->getCamera().getViewMatrix()),
            glm::value_ptr(m_cameraController->getCamera().getProjectionMatrix()));

        bgfx::touch(0);


        wm.deleteCreateChunks(m_renderer.getChunkManagerRef(), m_cameraController);


        m_renderer.render();

        // Advance to next frame. Process submitted rendering primitives.
        bgfx::frame();

        keyboard.updateKeys();
        mouse.updatePos();
    }
}

void App::terminate() {
    m_renderer.terminate();
    bgfx::shutdown();
    m_window.terminate();
}