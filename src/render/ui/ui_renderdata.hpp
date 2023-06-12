//
// Created by adeon on 6/12/23.
//

#ifndef COOL_GAME_UI_RENDERDATA_HPP
#define COOL_GAME_UI_RENDERDATA_HPP

#include <bgfx/bgfx.h>
#include <glm/glm.hpp>

namespace rend {
    namespace ui {
        struct QuadTexVertex {
            QuadTexVertex(const glm::vec3& p, const glm::vec2& uv): pos{p}, texCoords{uv} {}

            glm::vec3 pos;
            glm::vec2 texCoords;

            QuadTexVertex() = default;

            static void init();

            // storage in chunk_renderer.cpp
            static bgfx::VertexLayout layout;
        };

        const QuadTexVertex FULLSCREEN_TEX_QUAD_VERTICES[4] =
        {
            {glm::vec3{-1.0f,  1.0f,  0.0f}, glm::vec2{0.0f, 0.0f} },
            {glm::vec3{ 1.0f,  1.0f,  0.0f}, glm::vec2{1.0f, 0.0f} },
            {glm::vec3{ 1.0f, -1.0f,  0.0f}, glm::vec2{1.0f, 1.0f} },
            {glm::vec3{-1.0f, -1.0f,  0.0f}, glm::vec2{0.0f, 1.0f} },
        };

        const uint16_t FULLSCREEN_TEX_QUAD_INDICES[] {
            3, 1, 0, 3, 2, 1
        };
    } // ui
} // rend

#endif //COOL_GAME_UI_RENDERDATA_HPP
