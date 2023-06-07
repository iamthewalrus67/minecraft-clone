#include <iostream>

#include <bx/bx.h>
#include <bx/math.h>

#include "cube_renderer.hpp"

#include "shader_loading/shader_loading.hpp"

namespace rend {

    //! Shits for testing, or is it??
    static CubeRenderer::CubeVertex s_cubeVertices[8] =
            {
                    {glm::vec3{-1.0f,  1.0f,  1.0f}, glm::vec3{1.0f, 0.0f, 0.0f} },
                    {glm::vec3{ 1.0f,  1.0f,  1.0f}, glm::vec3{1.0f, 0.0f, 0.0f} },
                    {glm::vec3{-1.0f, -1.0f,  1.0f}, glm::vec3{1.0f, 0.0f, 0.0f} },
                    {glm::vec3{ 1.0f, -1.0f,  1.0f}, glm::vec3{1.0f, 0.0f, 0.0f} },
                    {glm::vec3{-1.0f,  1.0f, -1.0f}, glm::vec3{1.0f, 0.0f, 0.0f} },
                    {glm::vec3{ 1.0f,  1.0f, -1.0f}, glm::vec3{1.0f, 0.0f, 0.0f} },
                    {glm::vec3{-1.0f, -1.0f, -1.0f}, glm::vec3{0.0f, 0.0f, 1.0f} },
                    {glm::vec3{ 1.0f, -1.0f, -1.0f}, glm::vec3{0.0f, 0.0f, 1.0f} },
            };

    static const uint16_t s_cubeIndices[] = {
            1, 0, 2,
            3, 1, 2,
            6, 4, 5,
            6, 5, 7,
            2, 0, 4,
            2, 4, 6,
            5, 1, 3,
            7, 5, 3,
            4, 0, 1,
            5, 4, 1,
            3, 2, 6,
            3, 6, 7
    };

    bgfx::VertexLayout CubeRenderer::CubeVertex::layout;

    void CubeRenderer::CubeVertex::init() {
        layout
                .begin()
                .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
                .add(bgfx::Attrib::Color0,   3, bgfx::AttribType::Float, true)
                .end();
    }

    void CubeRenderer::init() {
        // Create vertex stream declaration.
        CubeVertex::init();

        // Create static vertex buffer.
        m_vbh = bgfx::createVertexBuffer(
                bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices) )
                , CubeVertex::layout
        );

        // Create static index buffer.
        m_ibh = bgfx::createIndexBuffer(
                bgfx::makeRef(s_cubeIndices, sizeof(s_cubeIndices) )
        );

        bgfx::ShaderHandle vsh = loadShader("res/shaders/instancing/vs_instancing.bin");
        bgfx::ShaderHandle fsh = loadShader("res/shaders/instancing/fs_instancing.bin");

        m_program = bgfx::createProgram(vsh, fsh, true);
    }

    void CubeRenderer::render() {
        // 80 bytes stride = 64 bytes for 4x4 matrix + 16 bytes for RGBA color.
        const uint16_t instanceStride = 80;
        // to total number of instances to draw
        const uint32_t width = 32;
        const uint32_t length = 32;
        const uint32_t totalCubes = width * length;

        // figure out how big of a buffer is available
        uint32_t drawnCubes = bgfx::getAvailInstanceDataBuffer(totalCubes, instanceStride);

        // save how many we couldn't draw due to buffer room so we can display it
        auto missing = totalCubes - drawnCubes;
        std::cout << missing << std::endl;

        bgfx::InstanceDataBuffer idb;
        bgfx::allocInstanceDataBuffer(&idb, drawnCubes, instanceStride);

        uint8_t* data = idb.data;

        for (uint32_t ii = 0; ii < drawnCubes; ++ii)
        {
            uint32_t yy = ii / width;
            uint32_t xx = ii % width;

            float* mtx = (float*)data;
            bx::mtxRotateY(mtx, 0.00f);
            mtx[12] = -15.0f + float(xx) * 2.0f;
            mtx[13] = -15.0f + float(yy) * 2.0f;
            mtx[14] = 0.0f;

            float* color = (float*)&data[64];
            color[0] = 1.0f;
            color[1] = 0.0f;
            color[2] = 0.0f;
            color[3] = 1.0f;

            data += instanceStride;
        }

        // Set vertex and index buffer.
        bgfx::setVertexBuffer(0, m_vbh);
        bgfx::setIndexBuffer(m_ibh);

        // Set instance data buffer.
        bgfx::setInstanceDataBuffer(&idb);

        // Set render states.
        bgfx::setState(BGFX_STATE_DEFAULT);

        // Submit primitive for rendering to view 0.
        bgfx::submit(0, m_program);
    }

} // rend
