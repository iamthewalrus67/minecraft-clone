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
    }

    void MasterRenderer::render() {
        bgfx::setTexture(0, m_blockTexSampler, m_blockTexture.handle, defaultSamplerFlags);

        m_chunkManager.reMeshChunks();

        auto& chunkRenderers = m_chunkManager.getChunkRenderers();
        for (auto& renderer: chunkRenderers) {
            renderer.second.render();

            bgfx::submit(0, m_chunkProgram);
        }
    }

    void MasterRenderer::terminate() {
        bgfx::destroy(m_blockTexSampler);
        bgfx::destroy(m_blockTexture.handle);
        bgfx::destroy(m_chunkProgram);
        m_chunkManager.terminate();
    }
} // rend