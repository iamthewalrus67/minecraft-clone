#ifndef APP_HPP
#define APP_HPP

#include "window/window.hpp"
#include "render/renderer/cube_renderer.hpp"

class App {
public:
    App(const char *title, uint32_t width, uint32_t height);
    ~App();

    void start();
    void init();
    void terminate();
private:
    WindowWrapper m_window;
    rend::CubeRenderer m_renderer;
};

#endif // APP_HPP
