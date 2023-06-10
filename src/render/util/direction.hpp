//
// Created by adeon on 6/7/23.
//

#ifndef COOL_GAME_DIRECTION_HPP
#define COOL_GAME_DIRECTION_HPP

#include <bgfx/bgfx.h>

#include <glm/glm.hpp>

namespace util {

    static constexpr inline glm::vec3 DIRECTION_TO_VEC3[] {
        glm::vec3{0, 0, 1},
        glm::vec3{0, 0, -1},
        glm::vec3{1, 0, 0},
        glm::vec3{-1, 0, 0},
        glm::vec3{0, 1, 0},
        glm::vec3{0, -1, 0},
    };

    struct Direction {
        enum INDEX {
            FRONT  = 0,         // +z
            BACK   = 1,         // -z
            RIGHT  = 2,         // +x
            LEFT   = 3,         // -x
            TOP    = 4,         // +y
            BOTTOM = 5,         // -y
        };

        //! TODO: VERY SLOOOOOOOOOWWWW
        glm::vec3 toGlmVec3() {
            return DIRECTION_TO_VEC3[idx];
        }

        bool operator==(INDEX i) { return idx == i; }

        INDEX idx;
    };
} // util


#endif //COOL_GAME_DIRECTION_HPP
