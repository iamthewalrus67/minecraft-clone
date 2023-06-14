//
// Created by iamthewalrus on 6/10/23.
//

#include <glm/gtx/string_cast.hpp>

#include "player.hpp"
#include "math/ray.hpp"
#include "time/time.hpp"

namespace player {

math::AABBf Player::aabb() {
    return math::AABBf::unit()
                        .scale(glm::vec3(0.5f, MovementSettings::HEIGHT, 0.5f))
                        .translate(glm::vec3{0.0f, m_position.y, 0.0f})
                        .centerOn(m_position, glm::bvec3{1, 0, 1});
}

float Player::tryMoveAxis(const math::AABBf& box, float movement, const std::span <math::AABBf>& colliders,
                          const glm::vec3& axis) {
    constexpr float EPSILON = 0.05f;

    auto d_v = axis * movement;
    auto sign = glm::sign(movement);
    auto index = axis.x != 0 ? 0 : (axis.y != 0 ? 1 : 2);

    auto moved = box.translate(d_v);

    for (const auto &c : colliders) {
        if (!c.collides(moved)) {
            continue;
        }

        auto depth = moved.depth(c)[index];
        d_v[index] += -sign * (depth + EPSILON);
        moved = box.translate(d_v);

        if (glm::abs(d_v[index]) <= EPSILON) {
            d_v[index] = 0.0f;
            break;
        }
    }

    auto result = d_v[index];
    return glm::abs(result) <= glm::epsilon<float>() ? 0.0f : result;
}

glm::vec3 Player::tryMove(const math::AABBf& box, const glm::vec3& movement, const std::span<math::AABBf>& colliders) {
    glm::vec3 result;
    auto current = box;

    for (size_t i = 0; i < 3; ++i) {
        glm::vec3 axis{0};
        axis[i] = 1.0f;
        float movementAxis = tryMoveAxis(box, movement[i], colliders, axis);
        current = current.translate(axis * movementAxis);
        result[i] = movementAxis;
    }

    return result;
}

void Player::update() {
    Keyboard &keyboard = Keyboard::instance();
    Mouse &mouse = Mouse::instance();

    for (auto &[key, val] : KEY_BLOCKS_MAP) {
        if (keyboard.isPressed(key)) {
            m_currentBlock = val;
        }
    }

    // Movement
    glm::vec3 direction = glm::vec3{0.0f, 0.0f, 0.0f};
    for (auto &key : MovementSettings::MOVEMENT_BINDS) {
        if (keyboard.isPressed(key)) {
            direction += MovementSettings::MOVEMENT_BIND_MAP[key];
        }
    }

    if (keyboard.isJustPressed(GLFW_KEY_V)) {
        m_noClip = !m_noClip;
    }

    if (glm::length(direction) > 0) {
        direction = glm::normalize(direction);
    }

    if (keyboard.isPressed(GLFW_KEY_LEFT_SHIFT)) {
        m_speed = m_flying ? MovementSettings::SLOW_FLYING_SPEED : MovementSettings::SLOW_MOVEMENT_SPEED;
    } else {
        m_speed = m_flying ? MovementSettings::FLYING_SPEED : MovementSettings::MOVEMENT_SPEED;
    }

    bool jumpPressed = keyboard.isPressed(GLFW_KEY_SPACE);

    if (jumpPressed && m_flying) {
        direction.y = 1.0f;
    }

    auto deltaTime = util::Time::instance().deltaTime();
    glm::vec3 movement = direction * m_speed * deltaTime;

    m_velocity += movement;

    if (jumpPressed && !m_flying && m_grounded) {
        m_velocity.y += 5.0f * deltaTime;
    }

    if (keyboard.isJustPressed(GLFW_KEY_G)) {
        m_flying = !m_flying;
    }

    if (!m_flying && !m_grounded) {
        m_velocity.y -= MovementSettings::GRAVITY * deltaTime;
    }

    m_velocity.x -= MovementSettings::DRAG_COEFFICIENT * m_velocity.x;
    m_velocity.z -= MovementSettings::DRAG_COEFFICIENT * m_velocity.z;
    if (m_flying) {
        m_velocity.y -= MovementSettings::DRAG_COEFFICIENT * m_velocity.y;
    }

    m_velocity.x = std::clamp(m_velocity.x, -m_speed, m_speed);
    m_velocity.z = std::clamp(m_velocity.z, -m_speed, m_speed);
    m_velocity.y = m_flying ? std::clamp(m_velocity.y, -m_speed, m_speed) : std::clamp(m_velocity.y, -10.0f, m_speed);

    std::cout << "Velocity: " << glm::to_string(m_velocity) << std::endl;

    // Collision detection

    auto newPosition = m_position;
    auto rightDir = m_cameraController->getRightDir();
    auto frontDir = -glm::cross(rightDir, glm::vec3{0.0f, 1.0f, 0.0f});
    frontDir = glm::normalize(frontDir);
    auto rotatedVelocity = rightDir * m_velocity.x
                            + glm::vec3{0.0f, 1.0f, 0.0f} * m_velocity.y
                            + frontDir * m_velocity.z;

    glm::vec3 legalMove = rotatedVelocity;
    if (!m_noClip) {
        std::array<math::AABBf, 256> colliders;
        size_t n = m_chunkManager.getColliders(colliders,
                                               math::AABBi::unit().scale(4).centerOn(glm::floor(m_position)));
        legalMove = tryMove(aabb(), rotatedVelocity, {&colliders[0], n});
    }

    glm::bvec3 stop{false};
    for (size_t i = 0; i < 3; ++i) {
        if (glm::abs(rotatedVelocity[i] - legalMove[i]) >= glm::epsilon<float>()) {
            stop[i] = true;
        }
    }

    newPosition += legalMove;

    m_cameraController->setPosition(newPosition + glm::vec3{0.0f, MovementSettings::HEIGHT, 0.0f});
    m_position = newPosition;

    m_grounded = stop.y;
    if (m_grounded) {
        m_velocity.y = 0.0f;
    }

    // Block destruction
    bool shouldMine = mouse.isLeftButtonJustPressed();
    bool shouldPlace = mouse.isRightButtonJustPressed();
    if (shouldMine || shouldPlace) {
        auto intersection = math::Ray{m_cameraController->getPosition(), m_cameraController->getDirection()}
            .intersectBlock([&](auto p) {
            auto chunk = m_chunkManager.getChunkRefFromGlobalPos(p);
            if (!chunk) {
                return false;
            }
            auto blockId = chunk->getBlockDataFromGlobalPos(p).blockID;
            return blockId != rend::BLOCKS::AIR && blockId != rend::BLOCKS::WATER;
        }, MINING_DISTANCE);

        if (intersection) {
            auto chunk =
                    m_chunkManager.getChunkRefFromGlobalPos(shouldMine ?
                                                            intersection->position :
                                                            intersection->position + intersection->direction);
            if (chunk) {
                auto block =
                        chunk->getBlockDataFromGlobalPos(shouldMine ?
                        intersection->position :
                        intersection->position + intersection->direction);

                chunk->setBlockByPlayer(block.localChunkPos, shouldMine ?
                                                    rend::BLOCKS::AIR :
                                                    m_currentBlock);
            }
        }
    }
}

} // player