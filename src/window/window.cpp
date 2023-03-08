#include <iostream>

#include <GLFW/glfw3native.h>
#include <sstream>

#include "GLFW/glfw3.h"
#include "logging/logger/logger.hpp"

#include "input/keyboard.hpp"
#include "input/mouse.hpp"
#include "logging/error_handling/error_handling.hpp"
#include "window.hpp"

static void glfw_errorCallback(int error, const char *description) {
    std::stringstream ss;
    ss << "GLFW Error " << error << ": " << description;
    Logger::instance().logErr(ss.str());
}

static void glfw_keyCallback(GLFWwindow *window, int key, int scancode,
                             int action, int mods) {
    auto &keyboard = Keyboard::instance();
    keyboard.setKeyAction(key, action);
}

static void glfw_mouseCursorCallback(GLFWwindow *window, double xpos,
                                     double ypos) {
    Mouse &mouse = Mouse::instance();
    mouse.setPos(xpos, ypos);
}

WindowWrapper::WindowWrapper(const char *title, uint32_t width,
                             uint32_t height) {
    glfwSetErrorCallback(glfw_errorCallback);
    if (!glfwInit()) {
        logAndExit("Failed to initialize glfw");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!m_window) {
        logAndExit("Failed to create glfw window");
    }
    glfwSetKeyCallback(m_window, glfw_keyCallback);
    glfwSetCursorPosCallback(m_window, glfw_mouseCursorCallback);
}
