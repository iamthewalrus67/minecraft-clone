//
// Created by adeon on 6/2/23.
//

#ifndef COOL_GAME_CUBE_RENDERER_HPP
#define COOL_GAME_CUBE_RENDERER_HPP

#include <glm/glm.hpp>

#include <bgfx/bgfx.h>

#include "render/chunk/chunk.hpp"


namespace rend {
    class CubeRenderer {
    public:
        struct CubeVertex {
            CubeVertex(const glm::vec3& p, const glm::vec3& c): pos{p}, color{c} {}

            glm::vec3 pos;
            glm::vec3 color;

            CubeVertex() = default;

            static void init();

            // storage in chunk_renderer.cpp
            static bgfx::VertexLayout layout;
        };
        //! Init all the internal structs
        void init();
        //! AAAAA
        void render();
    private:
        bgfx::VertexBufferHandle m_vbh;
        bgfx::IndexBufferHandle  m_ibh;
        bgfx::ProgramHandle m_program;
        Chunk m_chunk{glm::vec3{16.0f, 0.0f, 0.0f}};
    };
} // rend

#endif //COOL_GAME_CUBE_RENDERER_HPP
