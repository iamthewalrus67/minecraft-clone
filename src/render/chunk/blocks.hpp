//
// Created by adeon on 6/7/23.
//

#ifndef COOL_GAME_BLOCKS_HPP
#define COOL_GAME_BLOCKS_HPP

#include <bgfx/bgfx.h>
#include <glm/glm.hpp>

#include "unordered_map"
#include <string>

#include "render/util/direction.hpp"

namespace rend {
    enum BLOCKS {
        AIR = 0,
        GRASS = 1,
        SAND = 2,
        STONE = 3,
        OAK_WOOD = 4,
        OAK_LEAVES = 5,
        COBBLESTONE = 6,
        COAL = 7,
        IRON = 8,
        SNOW = 9,
        SNOW_POWDER = 10,
        WATER = 11,
        LAVA = 12,
        DIRT = 13
    };

    inline std::string getBlockNameFromBockID(BLOCKS blockID) {
        switch(blockID) {
            case AIR:
                return "AIR";
                break;
            case GRASS:
                return "GRASS";
                break;
            case SAND:
                return "SAND";
                break;
            case STONE:
                return "STONE";
                break;
            case OAK_WOOD:
                return "OAK_WOOD";
                break;
            case OAK_LEAVES:
                return "OAK_LEAVES";
                break;
            case COBBLESTONE:
                return "COBBLESTONE";
                break;
            case IRON:
                return "IRON";
                break;
            case SNOW:
                return "SNOW";
                break;
            case SNOW_POWDER:
                return "SNOW_POWDER";
                break;
            case WATER:
                return "WATER";
                break;
            case LAVA:
                return "LAVA";
                break;
            case DIRT:
                return "DIRT";
                break;
            case COAL:
                return "COAL";
                break;
            default:
                return "IDK";
                break;
        }

        return "IDK";
    }

    //! Get the offset in texture depending on block type
    [[nodiscard]] inline glm::vec2 getTextureOffset(BLOCKS ID, util::Direction dir) {
        switch (ID) {
            case GRASS:
                if (dir == util::Direction::TOP) { return glm::vec2{0, 0}; }
                else if (dir == util::Direction::BOTTOM) { return glm::vec2{2, 0}; }
                else { return glm::vec2{1, 0}; }
            case SAND:
                return glm::vec2{0, 1};
            case STONE:
                return glm::vec2{3, 0};
            case OAK_WOOD:
                if (dir == util::Direction::TOP || dir == util::Direction::BOTTOM) { return glm::vec2{3, 1}; }
                else { return glm::vec2{2, 1}; }
            case OAK_LEAVES:
                return glm::vec2{4, 1};
            case COBBLESTONE:
                return glm::vec2{2, 2};
            case COAL:
                return glm::vec2{4, 0};
            case IRON:
                return glm::vec2{5, 0};
            case SNOW:
                return glm::vec2{3, 2};
            case SNOW_POWDER:
                return glm::vec2{3, 2};
            case WATER:
                return glm::vec2{0, 15};
            case LAVA:
                return glm::vec2{0, 14};
            case DIRT:
                return glm::vec2{2, 0};
            default:
                return glm::vec2{0, 0};
        }
    }
}

#endif //COOL_GAME_BLOCKS_HPP
