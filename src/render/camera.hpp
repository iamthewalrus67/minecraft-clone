#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "glm/fwd.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace rend {
class Camera {
public:
    Camera(float fov, int screenWidth, int screenHeight,
           const glm::vec3 &position);

    void setProjectionMatrix(float fov, float screenWidth, float screenHeight);
    void addRotation(const glm::vec3 &angles);
    void addRelativeOffset(const glm::vec3 &offset);

    [[nodiscard]] glm::mat4 getProjectionMatrix() { return m_projection; };
    [[nodiscard]] glm::mat4 getViewMatrix() { return m_view; };

private:
    //! Update view matrix, called at every camera state change
    void updateViewMatrix();

    static constexpr glm::vec3 FRONT_DIR = glm::vec3{0.0f, 0.0f, -1.0f};
    static constexpr glm::vec3 UP_DIR = glm::vec3{0.0f, 1.0f, 0.0f};
    static constexpr glm::vec3 RIGHT_DIR = glm::vec3{1.0f, 0.0f, 0.0f};

    glm::vec3 m_frontDir;
    glm::vec3 m_rightDir;
    glm::vec3 m_upDir;
    glm::vec3 m_position;

    glm::vec3 m_rotation;

    glm::mat4 m_projection;
    glm::mat4 m_view;
};
} // namespace rend

#endif
