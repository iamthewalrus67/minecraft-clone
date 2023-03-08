#include <glm/gtx/euler_angles.hpp>
#include <iostream>

#include "camera.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/gtx/string_cast.hpp"

namespace rend {
Camera::Camera(float fov, int screenWidth, int screenHeight,
               const glm::vec3 &position)
    : m_position{position}, m_rotation{0.0f, 0.0f, 0.0f} {
    setProjectionMatrix(fov, screenWidth, screenHeight);
    updateViewMatrix();
}

void Camera::setProjectionMatrix(float fov, float screenWidth,
                                 float screenHeight) {
    m_projection = glm::perspective(glm::radians(fov),
                                    static_cast<float>(screenWidth) /
                                        static_cast<float>(screenHeight),
                                    0.1f, 100.0f);
}

void Camera::addRotation(const glm::vec3 &angles) {
    m_rotation += angles;
    if (m_rotation.x >= glm::pi<float>()) {
        m_rotation.x = glm::pi<float>();
    } else if (m_rotation.x <= -glm::pi<float>()) {
        m_rotation.x = glm::pi<float>();
    }
    updateViewMatrix();
}
void Camera::addRelativeOffset(const glm::vec3 &offset) {
    m_position +=
        m_rightDir * offset.x + m_upDir * offset.y + m_frontDir * offset.z;
    updateViewMatrix();
}

void Camera::updateViewMatrix() {
    glm::mat4 rotationMat =
        glm::eulerAngleYXZ(m_rotation.y, m_rotation.x, m_rotation.z);

    m_frontDir = rotationMat * glm::vec4(FRONT_DIR, 0.0f);

    m_upDir = rotationMat * glm::vec4(UP_DIR, 0.0f);

    m_rightDir = rotationMat * glm::vec4(RIGHT_DIR, 0.0f);

    m_view = glm::lookAt(m_position, m_position + m_frontDir, m_upDir);
    std::cout << glm::to_string(m_view) << std::endl;
}
} // namespace rend