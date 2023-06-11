#include <algorithm>
#include "flying_camera_controller.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "input/keyboard.hpp"
#include "input/mouse.hpp"
#include "render/camera/camera.hpp"

namespace control {
FlyingCameraController::FlyingCameraController(float fovDeg,
                                               std::pair<int, int> screenWH,
                                               const glm::vec3 &position)
    : m_camera{fovDeg, screenWH.first, screenWH.second, position} {}

void FlyingCameraController::captureInputAndApply() {
//    handleMovement();
    handleRotation();
}

void FlyingCameraController::updateScreenSize(float screenWidth,
                                              float screenHeight) {
    m_camera.updateScreenSize(screenWidth, screenHeight);
}

void FlyingCameraController::moveCamera(const glm::vec3 &offset) {
    m_camera.addRelativeOffset(offset);
}

void FlyingCameraController::handleRotation() {

    glm::vec3 rotation{0.0f, 0.0f, 0.0f};

    rotation.x -= Mouse::instance().getYMovement() * 0.001f;
    rotation.y -= Mouse::instance().getXMovement() * 0.001f;

    if (glm::length(rotation) > 0) {
        m_camera.addRotation(rotation);
    }

}

glm::vec3 FlyingCameraController::getPosition(){
    return m_camera.getPosition();
}

glm::vec3 FlyingCameraController::getDirection() {
    return m_camera.getFrontDirection();
}

} // namespace control
