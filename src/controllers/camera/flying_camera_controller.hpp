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
    glm::vec3 getRightDir();
    glm::vec3 getUpDir();

    void setPosition(const glm::vec3& pos) {
        m_camera.setPosition(pos);
    }

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
