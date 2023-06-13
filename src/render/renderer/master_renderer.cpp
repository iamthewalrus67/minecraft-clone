//
// Created by adeon on 6/8/23.
//

#include "master_renderer.hpp"

#include "util/util.hpp"

#include "shader_loading/shader_loading.hpp"

namespace rend {
    void MasterRenderer::init() {
        m_blockTexture = ul::loadTexture("res/blocks.png");
        m_blockTexSampler = bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);

        // TODO: Temporary
        bgfx::ShaderHandle vsh = loadShader("res/shaders/vs_simple.bin");
        bgfx::ShaderHandle fsh = loadShader("res/shaders/fs_simple.bin");

        m_chunkProgram = bgfx::createProgram(vsh, fsh, true);

        vsh = loadShader("res/shaders/water/vs_simple.bin");
        fsh = loadShader("res/shaders/water/fs_simple.bin");

        m_chunkProgramTransparent = bgfx::createProgram(vsh, fsh, true);

        m_uiRenderer.init();
    }

    void MasterRenderer::render() {
        bgfx::setTexture(0, m_blockTexSampler, m_blockTexture.handle, defaultSamplerFlags);

        m_chunkManager.reMeshChunks();

        auto& chunkRenderers = m_chunkManager.getChunkRenderers();
        for (auto& renderer: chunkRenderers) {
            renderer.second.renderOpaque();

            bgfx::submit(0, m_chunkProgram);
        }

        for (auto& renderer: chunkRenderers) {
            renderer.second.renderTransparent();

            bgfx::submit(0, m_chunkProgramTransparent);
        }

        m_uiRenderer.render();
    }

    void MasterRenderer::setScreenSize(int screenWidth, int screenHeight) {
        m_screenWidth = screenWidth;
        m_screenHeight = screenHeight;

        m_uiRenderer.update(screenWidth, screenHeight);
    }

    void MasterRenderer::terminate() {
        bgfx::destroy(m_blockTexSampler);
        bgfx::destroy(m_blockTexture.handle);
        bgfx::destroy(m_chunkProgram);
        bgfx::destroy(m_chunkProgramTransparent);
        m_chunkManager.terminate();
        m_uiRenderer.terminate();
    }
} // rend