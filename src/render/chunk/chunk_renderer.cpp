#include "chunk_renderer.hpp"

#include "shader_loading/shader_loading.hpp"

namespace rend {
    bgfx::VertexLayout ChunkRenderer::ChunkVertex::layout;

    void ChunkRenderer::ChunkVertex::init() {
        layout
                .begin()
                .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
                .add(bgfx::Attrib::Color0,   3, bgfx::AttribType::Float, true)
                .end();
    }

    static glm::vec3 CHUNK_VERTICES[8] =
            {
                    glm::vec3{-1.0f,  1.0f,  1.0f},
                    glm::vec3{ 1.0f,  1.0f,  1.0f},
                    glm::vec3{-1.0f, -1.0f,  1.0f},
                    glm::vec3{ 1.0f, -1.0f,  1.0f},
                    glm::vec3{-1.0f,  1.0f, -1.0f},
                    glm::vec3{ 1.0f,  1.0f, -1.0f},
                    glm::vec3{-1.0f, -1.0f, -1.0f},
                    glm::vec3{ 1.0f, -1.0f, -1.0f},
            };

    static const size_t CHUNK_INDICES[] = {
            1, 0, 2,    // front face up +Z
            3, 1, 2,    // front face bottom +Z
            6, 4, 5,    // back face up -Z
            6, 5, 7,    // back face bottom -Z
            2, 0, 4,    // left face up -X
            2, 4, 6,    // left face bottom -X
            5, 1, 3,    // right face up +X
            7, 5, 3,    // right face bottom +X
            4, 0, 1,    // up face bottom +Y
            5, 4, 1,    // up face top +Y
            3, 2, 6,    // bottom face bottom -Y
            3, 6, 7     // bottom face top +Y
    };

    static const size_t FACE_INDICES[] = {2, 0, 1, 3, 2, 1};

    void ChunkRenderer::init() {
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

        initTestChunk(&m_chunk);
    }

    void ChunkRenderer::render() {
        m_vertices.clear();
        m_indices.clear();

        glm::vec3 blockPos;
        for (uint32_t w = 0; w < Chunk::WIDTH_X; ++w) {
            for (uint32_t h = 0; h < Chunk::HEIGHT_Y; ++h) {
                for (uint32_t d = 0; d < Chunk::DEPTH_Z; ++d) {
                    auto bIdx = glm::vec3{w, h, d};
                    m_chunk.positionOf(&blockPos, bIdx);

                    const size_t offset = m_vertices.size();

                    // emit vertices
                    int a[]{0, 1, 2, 3};
                    for (size_t i = 0; i < 4; i++) {
                        ChunkRenderer::ChunkVertex vertex;
                        vertex.pos = blockPos + CHUNK_VERTICES[a[i]];
                        vertex.color = glm::vec3{1.0f, 0.0f, 0.0f};
                        m_vertices.push_back(vertex);
                    }

                    // emit indices
                    for (size_t i : FACE_INDICES) {
                        m_indices.push_back(offset + i);
                    }

                }
            }
        }

        //! Update the dynamic vertex buffer if chunk changed
        bgfx::update(m_dynamicVBH, 0,
                     bgfx::copy(
                        &m_vertices[0],
                        m_vertices.size() * sizeof(m_vertices[0])));

        //! Update the dynamic index buffer if chunk changed
        bgfx::update(m_dynamicIBH, 0,
                     bgfx::copy(
                        &m_indices[0],
                        m_indices.size() * sizeof(m_indices[0])));

        // Set vertex and index buffer.
        bgfx::setVertexBuffer(0, m_dynamicVBH);
        bgfx::setIndexBuffer(m_dynamicIBH);

        // Set render states.
        bgfx::setState(BGFX_STATE_DEFAULT);

        // Submit primitive for rendering to view 0.
        bgfx::submit(0, m_programTemp);
    }
} // rend