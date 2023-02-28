#ifndef MOUSE_HPP
#define MOUSE_HPP

#include "input/keyboard.hpp"
#include <GLFW/glfw3.h>
#include <utility>

class Mouse {
public:
    static Mouse &instance() {
        static Mouse inst;
        return inst;
    }

    Mouse &operator=(const Mouse &key) = delete;
    Mouse(const Mouse &key) = delete;

    void setPos(double xpos, double ypos) {
        m_xpos = xpos;
        m_ypos = ypos;
    }

    void updatePos() {
        m_prevXpos = m_xpos;
        m_prevYpos = m_ypos;
    }

    double getXpos() { return m_xpos; }
    double getYpos() { return m_ypos; }
    double getXMovement() { return m_xpos - m_prevXpos; }
    double getYMovement() { return m_ypos - m_prevYpos; }
    std::pair<double, double> getMovement() {
        return {getXMovement(), getYMovement()};
    }
    std::pair<double, double> getPos() { return {m_xpos, m_ypos}; }

    [[nodiscard]] bool isLeftButtonPressed() {
        return Keyboard::instance().isPressed(GLFW_MOUSE_BUTTON_LEFT);
    }

    [[nodiscard]] bool isLeftButtonJustPressed() {
        return Keyboard::instance().isJustPressed(GLFW_MOUSE_BUTTON_LEFT);
    }

    [[nodiscard]] bool isRightButtonPressed() {
        return Keyboard::instance().isPressed(GLFW_MOUSE_BUTTON_RIGHT);
    }

    [[nodiscard]] bool isRightButtonJustPressed() {
        return Keyboard::instance().isJustPressed(GLFW_MOUSE_BUTTON_RIGHT);
    }

    [[nodiscard]] bool isMiddleButtonPressed() {
        return Keyboard::instance().isPressed(GLFW_MOUSE_BUTTON_MIDDLE);
    }

    [[nodiscard]] bool isMiddleButtonJustPressed() {
        return Keyboard::instance().isJustPressed(GLFW_MOUSE_BUTTON_MIDDLE);
    }

private:
    Mouse() {}

    double m_xpos, m_ypos;
    double m_prevXpos, m_prevYpos;
};

#endif // MOUSE_HPP
