#ifndef FLYING_CAMERA_COTROLLER_HPP
#define FLYING_CAMERA_COTROLLER_HPP

#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

#include <glm/glm.hpp>

#include "GLFW/glfw3.h"
#include "input/keyboard.hpp"
#include "input/mouse.hpp"
#include "render/camera/camera.hpp"

namespace control {
class FlyingCameraController {
    struct CameraSettings {
        static constexpr float MOVEMENT_SPEED = 1.0f;
        static constexpr float ROTATION_SPEED = 0.5f;
        inline static std::unordered_map<int, glm::vec3> MOVEMENT_BIND_MAP{
            {GLFW_KEY_A, {-1.0f, 0.0f, 0.0f}},
            {GLFW_KEY_D, {1.0f, 0.0f, 0.0f}},
            {GLFW_KEY_W, {0.0f, 0.0f, 1.0f}},
            {GLFW_KEY_S, {0.0f, 0.0f, -1.0f}},
            {GLFW_KEY_SPACE, {0.0f, 1.0f, 0.0f}},
            {GLFW_KEY_LEFT_CONTROL, {0.0f, -1.0f, 0.0f}},
        };
        inline static std::vector<int> MOVEMENT_BINDS{
            GLFW_KEY_A, GLFW_KEY_D,     GLFW_KEY_W,
            GLFW_KEY_S, GLFW_KEY_SPACE, GLFW_KEY_LEFT_CONTROL};
    };

public:
    FlyingCameraController(float fovDeg, std::pair<int, int> screenWH,
                           const glm::vec3 &position);

    //! Capture the input from the players mouse and keyboard and apply the
    //! respective movement to the camera
    void captureInputAndApply();

    //! Update the projection matrix of internal camera to fit the new screen
    //! size at resize
    void updateScreenSize(float screenWidth, float screenHeight);

    glm::vec3 getPosition();
    glm::vec3 getDirection();

    void moveCamera(const glm::vec3& offset);

    rend::Camera &getCamera() { return m_camera; }

private:
    rend::Camera m_camera;

private:
    //! Handle Camera movement
//    void handleMovement();
    //! Handle Camera rotation
    void handleRotation();
};
} // namespace control

#endif // FLYING_CAMERA_COTROLLER_HPP
