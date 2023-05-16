#ifndef APP_HPP
#define APP_HPP

#include "window/window.hpp"

class App {
public:
    App(const char *title, uint32_t width, uint32_t height);
    ~App();

    void start();
private:
    WindowWrapper m_window;

    void init();
    void terminate();
};

#endif // APP_HPP
