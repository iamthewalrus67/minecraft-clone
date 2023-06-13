//
// Created by adeon on 6/7/23.
//

#ifndef COOL_GAME_CHUNK_RENDERER_HPP
#define COOL_GAME_CHUNK_RENDERER_HPP

#include <vector>
#include <array>

#include <bgfx/bgfx.h>

#include "chunk.hpp"
#include "render/util/texture.hpp"

namespace rend {
    class ChunkRenderer {
    public:
        struct ChunkVertex {
            glm::vec3 pos;
            glm::vec3 normal;
            glm::vec2 uv;
            glm::vec3 color;

            ChunkVertex() = default;

            static void init();

            // storage in chunk_renderer.cpp
            static bgfx::VertexLayout layout;
        };

        //! Initialize the renderer and the chunk
        void init(const glm::vec3& chunkPos);

        //! Build the mesh if needed(was changed) and render the chunk mesh
        void renderOpaque();

        //! Just render the transparent meshes
        void renderTransparent();

        //! Mesh the chunk if it was changed to render it properly
        void meshChunk(const std::array<Chunk*, 6> neighborChunks);

        //! Free resources
        void terminate();

        //! Get the chunk reference
        [[nodiscard]] Chunk& getChunkRef() { return m_chunk; }
    private:
        static constexpr uint32_t ATLAS_TEXTURES_W = 16;
        static constexpr uint32_t ATLAS_TEXTURES_H = 16;
        static constexpr glm::vec2 TEXTURE_SIZE = glm::vec2{1.0f / static_cast<float>(ATLAS_TEXTURES_W),
                                                            1.0f / static_cast<float>(ATLAS_TEXTURES_H)};

        Chunk m_chunk;

        std::vector<ChunkVertex> m_opaqueVertices;
        std::vector<uint32_t> m_opaqueIndices;

        std::vector<ChunkVertex> m_transparentVertices;
        std::vector<uint32_t> m_transparentIndices;

        bgfx::DynamicVertexBufferHandle m_dynamicVBH;
        bgfx::DynamicIndexBufferHandle  m_dynamicIBH;

        bgfx::DynamicVertexBufferHandle m_dynamicTransparentVBH;
        bgfx::DynamicIndexBufferHandle  m_dynamicTransparentIBH;
    };
} // rend

#endif //COOL_GAME_CHUNK_RENDERER_HPP
