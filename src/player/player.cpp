//
// Created by iamthewalrus on 6/10/23.
//

#include "player.hpp"
#include "math/ray.hpp"

namespace player {

void Player::update() {
    Keyboard &keyboard = Keyboard::instance();
    Mouse &mouse = Mouse::instance();

    // Movement
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

    bool shouldMine = mouse.isLeftButtonJustPressed();
    bool shouldPlace = mouse.isRightButtonJustPressed();

    // Block destruction
    if (shouldMine || shouldPlace) {
        auto intersection = math::Ray{m_cameraController->getPosition(), m_cameraController->getDirection()}
            .intersectBlock([&](auto p) {
            auto chunk = m_chunkManager.getChunkRefFromGlobalPos(p);
            if (!chunk) {
                return false;
            }
            auto blockId = chunk->getBlockDataFromGlobalPos(p).blockID;
            return blockId != rend::BLOCKS::AIR;
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
                                                    rend::BLOCKS::SAND);
            }
        }
    }
}

} // player