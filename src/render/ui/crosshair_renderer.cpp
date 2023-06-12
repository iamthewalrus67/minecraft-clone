//
// Created by adeon on 6/12/23.
//

#include "util/util.hpp"
#include "shader_loading/shader_loading.hpp"

#include "crosshair_renderer.hpp"

#include "ui_renderdata.hpp"

namespace rend {
    namespace ui {

        void CrossHairRenderer::init() {
            QuadTexVertex::init();

            m_vbh = bgfx::createVertexBuffer(
                    bgfx::makeRef(FULLSCREEN_TEX_QUAD_VERTICES, sizeof(FULLSCREEN_TEX_QUAD_VERTICES) )
                    ,QuadTexVertex::layout
            );

            m_ibh = bgfx::createIndexBuffer(
                    bgfx::makeRef(FULLSCREEN_TEX_QUAD_INDICES, sizeof(FULLSCREEN_TEX_QUAD_INDICES) )
            );

            m_texture = ul::loadTexture("res/crosshair.png");
            m_texSampler = bgfx::createUniform("s_texCrosshair", bgfx::UniformType::Sampler);

            bgfx::ShaderHandle vsh = loadShader("res/shaders/crosshair/vs_crosshair.bin");
            bgfx::ShaderHandle fsh = loadShader("res/shaders/crosshair/fs_crosshair.bin");

            m_program = bgfx::createProgram(vsh, fsh, true);
        }

        void CrossHairRenderer::render() {
            bgfx::setTexture(0, m_texSampler, m_texture.handle);

            bgfx::setVertexBuffer(0, m_vbh);
            bgfx::setIndexBuffer(m_ibh);

            bgfx::setState(BGFX_STATE_DEFAULT | BGFX_STATE_BLEND_ALPHA);

            bgfx::submit(0, m_program);
        }

        void CrossHairRenderer::update(int w, int h) {
            bgfx::destroy(m_vbh);

            for (uint32_t i = 0; i < 4; ++i) {
                m_vertexData[i].pos.x = FULLSCREEN_TEX_QUAD_VERTICES[i].pos.x * 0.03f;
                m_vertexData[i].pos.y = FULLSCREEN_TEX_QUAD_VERTICES[i].pos.y * 0.03f * static_cast<float>(w) / static_cast<float>(h);
                m_vertexData[i].texCoords = FULLSCREEN_TEX_QUAD_VERTICES[i].texCoords;
            }

            m_vbh = bgfx::createVertexBuffer(
                    bgfx::makeRef(m_vertexData, sizeof(m_vertexData))
                    , QuadTexVertex::layout
            );
        }

        void CrossHairRenderer::terminate() {
            bgfx::destroy(m_texSampler);
            bgfx::destroy(m_vbh);
            bgfx::destroy(m_ibh);
            bgfx::destroy(m_texture.handle);
            bgfx::destroy(m_program);
        }
    } // ui
} // rend