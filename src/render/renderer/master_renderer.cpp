//
// Created by adeon on 6/8/23.
//

#include "master_renderer.hpp"

#include "util/util.hpp"

namespace rend {
    void MasterRenderer::init() {
        m_blockTexture = ul::loadTexture("res/blocks.png");
        m_blockTexSampler = bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);
    }

    void MasterRenderer::render() {
        bgfx::setTexture(0, m_blockTexSampler, m_blockTexture.handle);

        auto& chunkRenderers = m_chunkManager.getChunkRenderers();
        for (auto& renderer: chunkRenderers) {
            renderer.second.render();
        }
    }

    void MasterRenderer::terminate() {
        bgfx::destroy(m_blockTexSampler);
        bgfx::destroy(m_blockTexture.handle);
        m_chunkManager.terminate();
    }
} // rend