//
// Created by adeon on 6/8/23.
//

#include "util.hpp"

#include "ext/stb_image.h"

namespace ul {
    rend::Texture loadTexture(const std::string& path) {
        rend::Texture textureRes;

        // load image with stb_image
        glm::ivec2 size;
        int channels;
        stbi_set_flip_vertically_on_load(true);
        uint8_t *data = stbi_load(path.c_str(), &size.x, &size.y, &channels, 0);

        auto res =
                bgfx::createTexture2D(
                        size.x, size.y,
                        false, 1,
                        bgfx::TextureFormat::RGBA8,
                        BGFX_SAMPLER_U_CLAMP
                        | BGFX_SAMPLER_V_CLAMP
                        | BGFX_SAMPLER_MIN_POINT
                        | BGFX_SAMPLER_MAG_POINT,
                        bgfx::copy(data, size.x * size.y * channels));

        if (!bgfx::isValid(res)) {
            textureRes.initialized = false;
        } else {
            std::free(data);
            textureRes.size = size;
            textureRes.handle = res;
        }

        return textureRes;
    }
} // ul