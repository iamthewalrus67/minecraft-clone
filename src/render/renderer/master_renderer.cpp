//
// Created by adeon on 6/8/23.
//

#include "master_renderer.hpp"

#include "util/util.hpp"

namespace rend {
    void MasterRenderer::init() {
        m_chunkRendererTemp.init(glm::vec3{-16.0f, 0.0f, 32.0f});
        m_blockTexture = ul::loadTexture("res/blocks.png");
        m_blockTexSampler = bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);
    }

    void MasterRenderer::render() {
        bgfx::setTexture(0, m_blockTexSampler, m_blockTexture.handle);
        m_chunkRendererTemp.render();
    }

    void MasterRenderer::terminate() {
        bgfx::destroy(m_blockTexSampler);
        bgfx::destroy(m_blockTexture.handle);
        m_chunkRendererTemp.terminate();
    }
} // rend