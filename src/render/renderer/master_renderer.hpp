//
// Created by adeon on 6/8/23.
//

#ifndef COOL_GAME_MASTER_RENDERER_HPP
#define COOL_GAME_MASTER_RENDERER_HPP

#include "render/util/texture.hpp"
#include "render/chunk/chunk_manager.hpp"
#include "render/ui/ui_renderer.hpp"

namespace rend {
    class MasterRenderer{
        static constexpr uint64_t defaultSamplerFlags =
        BGFX_SAMPLER_MIN_POINT
        | BGFX_SAMPLER_MAG_POINT
        | BGFX_SAMPLER_MIP_POINT
        | BGFX_SAMPLER_U_CLAMP;
    public:
        //! Initialize all the basic data
        void init();

        //! Call render for each chunk
        void render();

        //! Free all the bgfx resources and call free on internal objects
        void terminate();

        //! Get the chunkmanager for further work
        [[nodiscard]] ChunkManager& getChunkManagerRef() { return m_chunkManager; }

        void setScreenSize(int screenWidth, int screenHeight);
    private:
        ChunkManager m_chunkManager;
        ui::UIRenderer m_uiRenderer;
        Texture m_blockTexture;
        bgfx::UniformHandle m_blockTexSampler;

        bgfx::ProgramHandle m_chunkProgram;

        int m_screenWidth, m_screenHeight;
    };
} // rend

#endif //COOL_GAME_MASTER_RENDERER_HPP
