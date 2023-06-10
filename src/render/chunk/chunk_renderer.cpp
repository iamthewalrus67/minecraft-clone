#include <iostream>

#include "chunk_renderer.hpp"

#include "render/util/direction.hpp"

#include "shader_loading/shader_loading.hpp"

namespace rend {
    bgfx::VertexLayout ChunkRenderer::ChunkVertex::layout;

    void ChunkRenderer::ChunkVertex::init() {
        layout
                .begin()
                .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
                .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float, true)
                .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
                .end();
    }


    // indices, within each list of 6 cube indices, which represent are the 4
    // unique vertices which make up each face (Taken from jdah)
    static const size_t UNIQUE_INDICES[] = {0, 1, 2, 5};

    // indices into emitted vertices which make up the two faces for a cube face
    //(Taken from jdah)
    static const size_t FACE_INDICES[] = {0, 1, 2, 0, 2, 3};

    static const size_t CHUNK_INDICES[] = {
            4, 7, 6, 4, 6, 5, // (south (+z))
            3, 0, 1, 3, 1, 2, // (north (-z))
            7, 3, 2, 7, 2, 6, // (east  (+x))
            0, 4, 5, 0, 5, 1, // (west  (-x))
            2, 1, 5, 2, 5, 6, // (up    (+y))
            0, 3, 7, 0, 7, 4  // (down  (-y))
    };

    static const glm::vec3 CHUNK_VERTICES[] = {
            glm::vec3(0, 0, 0),
            glm::vec3(0, 1, 0),
            glm::vec3(1, 1, 0),
            glm::vec3(1, 0, 0),
            glm::vec3(0, 0, 1),
            glm::vec3(0, 1, 1),
            glm::vec3(1, 1, 1),
            glm::vec3(1, 0, 1)
    };

    static const glm::vec3 CHUNK_NORMALS[] = {
            glm::vec3( 0,  0,  1),
            glm::vec3( 0,  0, -1),
            glm::vec3( 1,  0,  0),
            glm::vec3(-1,  0,  0),
            glm::vec3( 0,  1,  0),
            glm::vec3( 0, -1,  0),
    };

    static const glm::vec2 CHUNK_UVS[] = {
            glm::vec2(0, 0),
            glm::vec2(1, 0),
            glm::vec2(1, 1),
            glm::vec2(0, 1),
    };

    void ChunkRenderer::init(const glm::vec3& chunkPos) {
        ChunkVertex::init();

        m_dynamicVBH = bgfx::createDynamicVertexBuffer(
                16,
                ChunkVertex::layout,
                BGFX_BUFFER_ALLOW_RESIZE
        );

        // Create static index buffer.
        m_dynamicIBH = bgfx::createDynamicIndexBuffer(
                16,
                BGFX_BUFFER_ALLOW_RESIZE
                | BGFX_BUFFER_INDEX32
        );

        // TODO: Temporary
        bgfx::ShaderHandle vsh = loadShader("res/shaders/vs_simple.bin");
        bgfx::ShaderHandle fsh = loadShader("res/shaders/fs_simple.bin");

        m_programTemp = bgfx::createProgram(vsh, fsh, true);

        m_chunk.init(chunkPos);
    }

    void ChunkRenderer::render() {
        if (!m_chunk.isInitialized()) { return; }

        // Set vertex and index buffer.
        bgfx::setVertexBuffer(0, m_dynamicVBH);
        bgfx::setIndexBuffer(m_dynamicIBH);

        // Set render states.
        bgfx::setState(BGFX_STATE_DEFAULT);

        // Submit primitive for rendering to view 0.
        bgfx::submit(0, m_programTemp);
    }

    void ChunkRenderer::meshChunk(const std::array<Chunk*, 6> neighborChunks) {
        // Clear old mesh
        m_vertices.clear();
        m_indices.clear();

        glm::vec3 blockPos;
        for (uint32_t w = 0; w < Chunk::WIDTH_X; ++w) {
            for (uint32_t h = 0; h < Chunk::HEIGHT_Y; ++h) {
                for (uint32_t d = 0; d < Chunk::DEPTH_Z; ++d) {
                    // Get the block index within a chunk
                    auto bIdx = glm::vec3{w, h, d};
                    m_chunk.positionOf(&blockPos, bIdx);

                    BLOCKS blockID = m_chunk[bIdx];
                    // Iterate over all 6 directions
                    for (size_t i = 0; i < 6; ++i) {
                        util::Direction dir{static_cast<util::Direction::INDEX>(i)};
                        const size_t offset = m_vertices.size();

                        if (m_chunk.isOutOfBounds(bIdx + dir.toGlmVec3())) {
                            glm::ivec3 newChunkBIdx = bIdx + dir.toGlmVec3();
                            newChunkBIdx.x -= newChunkBIdx.x % Chunk::WIDTH_X;
                            newChunkBIdx.y -= newChunkBIdx.y % Chunk::HEIGHT_Y;
                            newChunkBIdx.z -= newChunkBIdx.z % Chunk::DEPTH_Z;
                            if (neighborChunks[dir.idx] && neighborChunks[dir.idx]->isBlockAir(newChunkBIdx)) {
                                continue;
                            }
                        }

                        // If neighbor is air and we are not air: emit a face
                        if (m_chunk.isBlockAir(bIdx + dir.toGlmVec3()) && !m_chunk.isBlockAir(bIdx)) {
                            glm::vec2 textureOffset = getTextureOffset(blockID, dir);
                            // emit vertices
                            for (size_t i = 0; i < 4; i++) {
                                ChunkRenderer::ChunkVertex vertex;
                                vertex.pos = blockPos + CHUNK_VERTICES[CHUNK_INDICES[(dir.idx * 6) + UNIQUE_INDICES[i]]];
                                vertex.normal = CHUNK_NORMALS[dir.idx];
                                vertex.uv = (CHUNK_UVS[i] * TEXTURE_SIZE) + glm::vec2(textureOffset.x, 16 - textureOffset.y - 1) * TEXTURE_SIZE;
                                m_vertices.push_back(vertex);
                            }

                            // emit indices
                            for (size_t i : FACE_INDICES) {
                                m_indices.push_back(offset + i);
                            }
                        }
                    }
                }
            }
        }

        // Do needed operations after chunk remesh
        m_chunk.logReMesh();
        if (m_vertices.size() == 0 || m_indices.size() == 0) { return; }

        // Update the dynamic vertex buffer if chunk changed
        bgfx::update(m_dynamicVBH, 0,
                     bgfx::copy(
                             &m_vertices[0],
                             m_vertices.size() * sizeof(m_vertices[0])));
        // Update the dynamic index buffer if chunk changed
        bgfx::update(m_dynamicIBH, 0,
                     bgfx::copy(
                             &m_indices[0],
                             m_indices.size() * sizeof(m_indices[0])));
    }

    void ChunkRenderer::terminate() {
        // Cleanup.
        bgfx::destroy(m_dynamicVBH);
        bgfx::destroy(m_dynamicIBH);
        bgfx::destroy(m_programTemp);
    }
} // rend