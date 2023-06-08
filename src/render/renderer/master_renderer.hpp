//
// Created by adeon on 6/8/23.
//

#ifndef COOL_GAME_MASTER_RENDERER_HPP
#define COOL_GAME_MASTER_RENDERER_HPP

#include "render/util/texture.hpp"
#include "render/chunk/chunk_renderer.hpp"

namespace rend {
    class MasterRenderer{
    public:
        void init();

        void render();
    private:
        ChunkRenderer m_chunkRendererTemp;

        Texture m_blockTexture;
        bgfx::UniformHandle m_blockTexSampler;
    };
} // rend

#endif //COOL_GAME_MASTER_RENDERER_HPP
