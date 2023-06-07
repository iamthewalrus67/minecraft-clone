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
            ChunkVertex(const glm::vec3& p, const glm::vec3& c): pos{p}, color{c} {}

            glm::vec3 pos;
            glm::vec3 color;

            ChunkVertex() = default;

            static void init();

            // storage in chunk_renderer.cpp
            static bgfx::VertexLayout layout;
        };

        //! Initialize the renderer
        void init();

        //! Build the mesh if needed(was changed) and render the chunk mesh
        void render();

        std::vector<ChunkVertex> m_vertices;
        std::vector<uint32_t> m_indices;

        bgfx::DynamicVertexBufferHandle m_dynamicVBH;
        bgfx::DynamicIndexBufferHandle  m_dynamicIBH;
        bgfx::ProgramHandle m_programTemp;
    private:
        Chunk m_chunk{glm::vec3{16.0f, 0.0f, -32.0f}};

        bool m_initedTemp = false;
    };
} // rend

#endif //COOL_GAME_CHUNK_RENDERER_HPP
