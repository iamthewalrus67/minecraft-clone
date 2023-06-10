//
// Created by iamthewalrus on 6/10/23.
//

#include "player.hpp"
#include "math/ray.hpp"

namespace player {

void Player::update() {
    Keyboard &keyboard = Keyboard::instance();
    Mouse &mouse = Mouse::instance();

    glm::vec3 offset = glm::vec3{0.0f, 0.0f, 0.0f};
    for (auto &key : MovementSettings::MOVEMENT_BINDS) {
        if (keyboard.isPressed(key)) {
            offset += MovementSettings::MOVEMENT_BIND_MAP[key];
        }
    }

    if (glm::length(offset) > 0) {
        offset = glm::normalize(offset);
        m_cameraController->moveCamera(offset);
    }

    // Block destruction
    if (mouse.isLeftButtonJustPressed()) {
        auto intersection = math::Ray{m_cameraController->getPosition(), m_cameraController->getDirection()}
            .intersectBlock([&](auto p) {
            auto chunk = m_chunkManager.getChunkRefFromGlobalPos(p);
            if (!chunk) {
                return false;
            }
            auto blockId = chunk->getBlockDataFromGlobalPos(p).blockID;
            return blockId != rend::BLOCKS::AIR;
        }, 4.0f);
        if (intersection) {
            auto chunk = m_chunkManager.getChunkRefFromGlobalPos(intersection->position);
            if (chunk) {
                auto block = chunk->getBlockDataFromGlobalPos(intersection->position);
                chunk->setBlock(block.localChunkPos, rend::BLOCKS::AIR);
            }
        }
    }
}

} // player