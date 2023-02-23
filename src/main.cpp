#include "bgfx/bgfx.h"
#include "bgfx/platform.h"

#include "SDL2/SDL.h"
#include "SDL2/SDL_syswm.h"

#include <iostream>

constexpr int WIDTH = 1600;
constexpr int HEIGHT = 900;

int main() {
    SDL_Window* window = nullptr;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL! SDL_Error: " << SDL_GetError() << std::endl;
    } else {
        window = SDL_CreateWindow("BGFX Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
        if (window == nullptr) {
            std::cerr << "Failed to create window! SDL_Error: " << SDL_GetError() << std::endl;
        }
    }

    SDL_SysWMinfo wmi;
    SDL_VERSION(&wmi.version);
    if (!SDL_GetWindowWMInfo(window, &wmi)) {
        return 1;
    }

    bgfx::PlatformData pd;
    pd.ndt = wmi.info.x11.display;
    pd.nwh = (void*)(uintptr_t)wmi.info.x11.window;

    bgfx::setPlatformData(pd);

//    bgfx::Init init;
//    init.type = bgfx::RendererType:;

    bgfx::renderFrame();
    bgfx::init();
    bgfx::reset(WIDTH, HEIGHT, BGFX_RESET_VSYNC);
    bgfx::setDebug(BGFX_DEBUG_NONE);
    bgfx::setViewRect(0, 0, 0, uint16_t(WIDTH), uint16_t(HEIGHT));

    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x443355FF, 1.0f, 0);
    bgfx::touch(0);

    bool quit = false;
    SDL_Event current_event;
    std::cerr << "Before loop" << std::endl;
    while (!quit) {
        while (SDL_PollEvent(&current_event) != 0) {
            if (current_event.type == SDL_QUIT) {
                quit = true;
            }
        }
        std::cerr << "Inside loop" << std::endl;
        bgfx::frame();
        std::cerr << "Inside loop 2" << std::endl;
    }
    std::cerr << "After loop" << std::endl;

    SDL_DestroyWindow(window);
    bgfx::shutdown();
    SDL_Quit();

    return 0;
}
