//
// Created by adeon on 6/7/23.
//

#ifndef COOL_GAME_BLOCKS_HPP
#define COOL_GAME_BLOCKS_HPP

#include <bgfx/bgfx.h>
#include <glm/glm.hpp>

#include "unordered_map"

#include "render/util/direction.hpp"

namespace rend {
    enum BLOCKS {
        AIR = 0,
        GRASS = 1,
        SAND = 2,
        STONE = 3
    };

    //! Get the offset in texture depending on block type
    [[nodiscard]] inline glm::vec2 getTextureOffset(BLOCKS ID, util::Direction dir) {
        if (ID == BLOCKS::GRASS) {
            if (dir == util::Direction::TOP) { return glm::vec2{0, 0}; }
            else if (dir == util::Direction::BOTTOM) { return glm::vec2{2, 0}; }
            else { return glm::vec2{1, 0}; }
        }

        if (ID == BLOCKS::SAND) {
            return glm::vec2{0, 1};
        }

        if (ID == BLOCKS::STONE) {
            return glm::vec2{3, 0};
        }
    }
}

#endif //COOL_GAME_BLOCKS_HPP
