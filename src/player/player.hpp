//
// Created by iamthewalrus on 6/10/23.
//

#ifndef COOL_GAME_PLAYER_HPP
#define COOL_GAME_PLAYER_HPP

#include "controllers/camera/flying_camera_controller.hpp"
#include "render/chunk/chunk_manager.hpp"

namespace player {

class Player {
private:
    static constexpr float MINING_DISTANCE = 4.0f;

    std::shared_ptr<control::FlyingCameraController> m_cameraController;
    rend::ChunkManager& m_chunkManager;

    glm::vec3 m_position;
    glm::vec3 m_velocity;

    float m_speed = MovementSettings::MOVEMENT_SPEED;
    bool m_flying = true;

    struct MovementSettings {
        static constexpr float GRAVITY = 0.05f;
        static constexpr float DRAG_COEFFICIENT = 0.2f;
        static constexpr float MOVEMENT_SPEED = 0.5f;
        static constexpr float ROTATION_SPEED = 0.5f;
        static constexpr float HEIGHT = 1.8f;
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

    float tryMoveAxis(const math::AABBf& box, float movement, std::span<math::AABBf> colliders, const glm::vec3& axis);
    glm::vec3 tryMove(const math::AABBf& box, const glm::vec3& movement, std::span<math::AABBf> colliders);

    math::AABBf aabb();
public:
    Player() = delete;

    Player(const glm::vec3& position, std::shared_ptr<control::FlyingCameraController> cameraController, rend::ChunkManager& chunkManager)
                    : m_cameraController(cameraController), m_chunkManager(chunkManager), m_position(position) {
        m_cameraController->setPosition(m_position);
    }

    void update();
};

}

#endif //COOL_GAME_PLAYER_HPP
