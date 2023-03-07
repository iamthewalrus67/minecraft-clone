#include "flying_camera_controller.hpp"

namespace control {
FlyingCameraController::FlyingCameraController(float fovDeg,
                                               std::pair<int, int> screenWH,
                                               const glm::vec3 &position)
    : m_camera{fovDeg, screenWH.first, screenWH.second, position} {}

void FlyingCameraController::captureInputAndApply() {}
} // namespace control
