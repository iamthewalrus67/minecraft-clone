//
// Created by adeon on 6/8/23.
//

#ifndef COOL_GAME_TEXTURE_HPP
#define COOL_GAME_TEXTURE_HPP

#include <bgfx/bgfx.h>
#include <glm/glm.hpp>

namespace rend {
    struct Texture {
        bgfx::TextureHandle handle;
        glm::ivec2 size;
        bool initialized = false;
    };
}

#endif //COOL_GAME_TEXTURE_HPP
