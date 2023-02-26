#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include <GLFW/glfw3.h>
#include <unordered_map>

class Keyboard {
public:
    static Keyboard &instance() {
        static Keyboard inst;
        return inst;
    }

    Keyboard &operator=(const Keyboard &key) = delete;
    Keyboard(const Keyboard &key) = delete;

    bool isJustPressed(int key) {
        return m_keyStatus[key] == GLFW_PRESS &&
               m_keyStatusPrev[key] == GLFW_RELEASE;
    }

    bool isJustReleased(int key) {
        return m_keyStatus[key] == GLFW_RELEASE &&
               m_keyStatusPrev[key] == GLFW_PRESS;
    }

    bool isPressed(int key) {
        return isJustPressed(key) || m_keyStatus[key] == GLFW_REPEAT;
    }

    int getKeyAction(int key) { return m_keyStatus[key]; }
    void setKeyAction(int key, int action) { m_keyStatus[key] = action; }

    void updateKeys() { m_keyStatusPrev = m_keyStatus; }

private:
    Keyboard() {}

    std::unordered_map<int, int> m_keyStatus{};
    std::unordered_map<int, int> m_keyStatusPrev{};
};

#endif // KEYBOARD_HPP
