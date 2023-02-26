#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <cstdint>
#include <utility>

#if BX_PLATFORM_LINUX
#define GLFW_EXPOSE_NATIVE_X11
#elif BX_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#elif BX_PLATFORM_OSX
#define GLFW_EXPOSE_NATIVE_COCOA
#endif
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

class WindowWrapper {
public:
    WindowWrapper(const char *title, uint32_t width, uint32_t height);
    ~WindowWrapper() = default;

    std::pair<uint32_t, uint32_t> getSize() {
        glfwGetWindowSize(m_window, &m_width, &m_height);
        return std::pair{m_width, m_height};
    }

    void getSize(int *width, int *height) {
        glfwGetWindowSize(m_window, width, height);
        m_width = *width;
        m_height = *height;
    }

    void terminate() { glfwTerminate(); }

    bool shouldClose() { return glfwWindowShouldClose(m_window); }

    void pollEvents() { glfwPollEvents(); }

    void *getWindow() {
#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
        return (void *)(uintptr_t)glfwGetX11Window(m_window);
#elif BX_PLATFORM_OSX
        return glfwGetCocoaWindow(m_window);
#elif BX_PLATFORM_WINDOWS
        return glfwGetWin32Window(m_window);
#endif
    }

    GLFWwindow *getGLFWWindow() { return m_window; }

private:
    int m_width, m_height;
    GLFWwindow *m_window;
};

#endif // WINDOW_HPP
