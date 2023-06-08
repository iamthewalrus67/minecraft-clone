
#ifndef COOL_GAME_UTIL_HPP
#define COOL_GAME_UTIL_HPP

#include <iostream>
#include <string>

#include <glm/glm.hpp>
#include <bgfx/bgfx.h>

#include "render/util/texture.hpp"

namespace ul {
    rend::Texture loadTexture(const std::string& path);
} // ul

#endif //COOL_GAME_UTIL_HPP
