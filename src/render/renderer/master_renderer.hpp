//
// Created by adeon on 6/8/23.
//

#ifndef COOL_GAME_MASTER_RENDERER_HPP
#define COOL_GAME_MASTER_RENDERER_HPP

#include "render/util/texture.hpp"
#include "render/chunk/chunk_manager.hpp"

namespace rend {
    class MasterRenderer{
    public:
        void init();

        void render();

        //! Free all the bgfx resources and call free on internal objects
        void terminate();
    private:
        ChunkManager m_chunkManager;
        Texture m_blockTexture;
        bgfx::UniformHandle m_blockTexSampler;
    };
} // rend

#endif //COOL_GAME_MASTER_RENDERER_HPP
