
#ifndef COOL_GAME_UTIL_HPP
#define COOL_GAME_UTIL_HPP

#include <iostream>
#include <string>

#include <glm/glm.hpp>
#include <bgfx/bgfx.h>

#include "render/util/texture.hpp"

namespace ul {
    rend::Texture loadTexture(const std::string& path);

    // The specialized hash function for `unordered_map` keys
    struct fuckingGlmHashFunctionIvec
    {
        std::size_t operator() (const glm::ivec3& v) const
        {
            std::size_t h1 = std::hash<int>()(v.x);
            std::size_t h2 = std::hash<int>()(v.y);
            std::size_t h3 = std::hash<int>()(v.z);

            return h1 ^ h2 ^ h3;
        }

        bool operator()(const glm::ivec3& a, const glm::ivec3& b)const
        {
            return a.x == b.x && a.y == b.y && a.z == b.z;
        }
    };
} // ul

#endif //COOL_GAME_UTIL_HPP
