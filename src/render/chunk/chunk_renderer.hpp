//
// Created by adeon on 6/7/23.
//

#ifndef COOL_GAME_CHUNK_RENDERER_HPP
#define COOL_GAME_CHUNK_RENDERER_HPP

#include <vector>

#include <bgfx/bgfx.h>

#include "chunk.hpp"

namespace rend {
    class ChunkRenderer {
    public:
        struct ChunkVertex {
            glm::vec3 pos;
            glm::vec3 normal;
            glm::vec2 uv;

            ChunkVertex() = default;

            static void init();

            // storage in chunk_renderer.cpp
            static bgfx::VertexLayout layout;
        };

        //! Initialize the renderer and the chunk
        void init(const glm::vec3& chunkPos);

        //! Build the mesh if needed(was changed) and render the chunk mesh
        void render();

        //! Get the chunk reference
        [[nodiscard]] Chunk& getChunkRef() { return m_chunk; }

        std::vector<ChunkVertex> m_vertices;
        std::vector<uint32_t> m_indices;

        bgfx::DynamicVertexBufferHandle m_dynamicVBH;
        bgfx::DynamicIndexBufferHandle  m_dynamicIBH;
        bgfx::ProgramHandle m_programTemp;
    private:
        //! Mesh the chunk if it was changed to render it properly
        void meshChunk();

        Chunk m_chunk;
    };
} // rend

#endif //COOL_GAME_CHUNK_RENDERER_HPP
