#ifndef FLYING_CAMERA_COTROLLER_HPP
#define FLYING_CAMERA_COTROLLER_HPP

#include <iostream>

#include <glm/glm.hpp>

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

private:
    rend::Camera m_camera;

private:
    //! Handle Camera movement
    void handleMovement();
    //! Handle Camera rotation
    void handleRotation();
};
} // namespace control

#endif // FLYING_CAMERA_COTROLLER_HPP
