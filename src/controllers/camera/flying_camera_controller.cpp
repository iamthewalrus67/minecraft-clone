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
    : camera{fovDeg, screenWH.first, screenWH.second, position} {}

void FlyingCameraController::captureInputAndApply() {
    handleMovement();
    handleRotation();
}

void FlyingCameraController::handleMovement() {
    glm::vec3 offset = glm::vec3{0.0f, 0.0f, 0.0f};
    for (auto &key : CameraSettings::MOVEMENT_BINDS) {
        if (Keyboard::instance().isPressed(key)) {
            std::cout << key << std::endl;
            offset += CameraSettings::MOVEMENT_BIND_MAP[key];
        }
    }

    if (glm::length(offset) > 0) {
        offset = glm::normalize(offset);
        camera.addRelativeOffset(offset);
    }
}

void FlyingCameraController::handleRotation() {

    glm::vec3 rotation{0.0f, 0.0f, 0.0f};

    if (!Mouse::instance().isLeftButtonPressed()) {
        rotation.x -= Mouse::instance().getYMovement() * 0.001f;
        rotation.y -= Mouse::instance().getXMovement() * 0.001f;
    }

    if (glm::length(rotation) > 0) {
        camera.addRotation(rotation);
    }
}

} // namespace control
