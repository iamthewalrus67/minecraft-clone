#include <iostream>

#include <GLFW/glfw3native.h>

#include "input/keyboard/keyboard.hpp"
#include "logging/error_handling/error_handling.hpp"
#include "window.hpp"

static void glfw_errorCallback(int error, const char *description) {
    fprintf(stderr, "GLFW error %d: %s\n", error, description);
}

static void glfw_keyCallback(GLFWwindow *window, int key, int scancode,
                             int action, int mods) {
    auto &inst = Keyboard::instance();
    inst.setKeyAction(key, action);
}

WindowWrapper::WindowWrapper(const char *title, uint32_t width,
                             uint32_t height) {
    glfwSetErrorCallback(glfw_errorCallback);
    if (!glfwInit()) {
        printAndExit("Failed to initialize glfw");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!m_window) {
        printAndExit("Failed to create glfw window");
    }
    glfwSetKeyCallback(m_window, glfw_keyCallback);
}
