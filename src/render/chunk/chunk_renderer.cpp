#include <iostream>

#include "chunk_renderer.hpp"

#include "render/util/direction.hpp"

namespace rend {
    bgfx::VertexLayout ChunkRenderer::ChunkVertex::layout;

    void ChunkRenderer::ChunkVertex::init() {
        layout
                .begin()
                .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
                .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float, true)
                .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
                .add(bgfx::Attrib::Color0, 3, bgfx::AttribType::Float)
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

    static const glm::vec3 DIRECTION_TO_COLOR[] = {
            glm::vec3(0.65f),
            glm::vec3(0.65f),
            glm::vec3(0.75f),
            glm::vec3(0.75f),
            glm::vec3(0.95f),
            glm::vec3(0.5f),
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

        m_dynamicTransparentVBH = bgfx::createDynamicVertexBuffer(
                16,
                ChunkVertex::layout,
                BGFX_BUFFER_ALLOW_RESIZE
        );

        // Create static index buffer.
        m_dynamicTransparentIBH = bgfx::createDynamicIndexBuffer(
                16,
                BGFX_BUFFER_ALLOW_RESIZE
                | BGFX_BUFFER_INDEX32
        );

        m_chunk.init(chunkPos);
    }

    void ChunkRenderer::renderOpaque() {
        if (!m_chunk.isInitialized()) { return; }

        // Set vertex and index buffer.
        bgfx::setVertexBuffer(0, m_dynamicVBH);
        bgfx::setIndexBuffer(m_dynamicIBH);

        // Set render states.
        bgfx::setState(BGFX_STATE_DEFAULT);
    }

    void ChunkRenderer::renderTransparent() {
        if (!m_chunk.isInitialized()) { return; }

        // Set vertex and index buffer.
        bgfx::setVertexBuffer(0, m_dynamicTransparentVBH);
        bgfx::setIndexBuffer(m_dynamicTransparentIBH);

        // Set render states.
        bgfx::setState(BGFX_STATE_DEFAULT | BGFX_STATE_BLEND_ALPHA);
    }

    void ChunkRenderer::meshChunk(const std::array<Chunk*, 6> neighborChunks) {
        // Clear old mesh
        m_opaqueVertices.clear();
        m_opaqueIndices.clear();
        m_transparentVertices.clear();
        m_transparentIndices.clear();

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

                        const size_t offset = m_opaqueVertices.size();
                        const size_t offsetTransparent = m_transparentVertices.size();

                        bool isNeighborLiquid = false;
                        // If neighbor is air and we are not air: emit a face
                        if ((m_chunk.isBlockAir(bIdx + dir.toGlmVec3()) || m_chunk.isBlockNotFull(bIdx + dir.toGlmVec3())) && (!m_chunk.isBlockAir(bIdx))) {

                            bool isTransparent = m_chunk.isBlockTransparent(bIdx);

                            if (m_chunk.isOutOfBounds(bIdx + dir.toGlmVec3())) {
                                if (!neighborChunks[dir.idx]) { continue; }
                                glm::ivec3 newChunkBIdx = bIdx + dir.toGlmVec3();
                                newChunkBIdx.x = newChunkBIdx.x % Chunk::WIDTH_X;
                                newChunkBIdx.y = newChunkBIdx.y % Chunk::HEIGHT_Y;
                                newChunkBIdx.z = newChunkBIdx.z % Chunk::DEPTH_Z;
                                isNeighborLiquid = neighborChunks[dir.idx]->isBlockNotFull(newChunkBIdx);
                                if (!neighborChunks[dir.idx]->isBlockAir(newChunkBIdx) && !isNeighborLiquid) {
                                    continue;
                                }
                            } else {
                                isNeighborLiquid = m_chunk.isBlockNotFull(bIdx + dir.toGlmVec3());
                            }
                            if (isNeighborLiquid && m_chunk.isBlockNotFull(bIdx)) { continue; }

                            glm::vec2 textureOffset = getTextureOffset(blockID, dir);
                            // emit vertices
                            for (size_t i = 0; i < 4; i++) {
                                ChunkRenderer::ChunkVertex vertex;
                                auto newBlockTopOffset = CHUNK_VERTICES[CHUNK_INDICES[(dir.idx * 6) +
                                                                                      UNIQUE_INDICES[i]]];
                                // TODO: SLOW
                                if (dir.idx == 4 && blockID == BLOCKS::WATER && !isNeighborLiquid) {
                                    newBlockTopOffset.y *= 0.9f;
                                    vertex.pos = blockPos + newBlockTopOffset;
                                } else if (blockID == BLOCKS::SNOW_POWDER) {
                                    newBlockTopOffset.y *= 0.1f;
                                    vertex.pos = blockPos + newBlockTopOffset;
                                } else {
                                    vertex.pos = blockPos + newBlockTopOffset;
                                }
                                vertex.normal = CHUNK_NORMALS[dir.idx];
                                vertex.uv = (CHUNK_UVS[i] * TEXTURE_SIZE) + glm::vec2(textureOffset.x, 16 - textureOffset.y - 1) * TEXTURE_SIZE;
                                vertex.color = DIRECTION_TO_COLOR[dir.idx];
                                if (isTransparent) {
                                    m_transparentVertices.push_back(vertex);
                                } else {
                                    m_opaqueVertices.push_back(vertex);
                                }
                            }
                            if (isTransparent) {
                                // emit indices
                                for (size_t i : FACE_INDICES) {
                                    m_transparentIndices.push_back(offsetTransparent + i);
                                }
                            } else {
                                // emit indices
                                for (size_t i : FACE_INDICES) {
                                    m_opaqueIndices.push_back(offset + i);
                                }
                            }
                        }
                    }
                }
            }
        }

        // Do needed operations after chunk remesh
        m_chunk.logReMesh();
        if (m_opaqueVertices.size() == 0 || m_opaqueIndices.size() == 0) { return; }

        // Update the dynamic vertex buffer if chunk changed
        bgfx::update(m_dynamicVBH, 0,
                     bgfx::copy(
                             &m_opaqueVertices[0],
                             m_opaqueVertices.size() * sizeof(m_opaqueVertices[0])));
        // Update the dynamic index buffer if chunk changed
        bgfx::update(m_dynamicIBH, 0,
                     bgfx::copy(
                             &m_opaqueIndices[0],
                             m_opaqueIndices.size() * sizeof(m_opaqueIndices[0])));

        if (m_transparentVertices.size() == 0 || m_transparentIndices.size() == 0) { return; }

        // Update the dynamic vertex buffer if chunk changed
        bgfx::update(m_dynamicTransparentVBH, 0,
                     bgfx::copy(
                             &m_transparentVertices[0],
                             m_transparentVertices.size() * sizeof(m_transparentVertices[0])));
        // Update the dynamic index buffer if chunk changed
        bgfx::update(m_dynamicTransparentIBH, 0,
                     bgfx::copy(
                             &m_transparentIndices[0],
                             m_transparentIndices.size() * sizeof(m_transparentIndices[0])));
    }

    void ChunkRenderer::terminate() {
        // Cleanup.
        bgfx::destroy(m_dynamicVBH);
        bgfx::destroy(m_dynamicIBH);
        bgfx::destroy(m_dynamicTransparentVBH);
        bgfx::destroy(m_dynamicTransparentIBH);
    }
} // rend