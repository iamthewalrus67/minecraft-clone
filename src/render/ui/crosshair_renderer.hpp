//
// Created by adeon on 6/12/23.
//

#ifndef COOL_GAME_CROSSHAIR_RENDERER_HPP
#define COOL_GAME_CROSSHAIR_RENDERER_HPP

#include <array>

#include <bgfx/bgfx.h>
#include <glm/glm.hpp>

#include "ui_renderdata.hpp"

#include "render/util/texture.hpp"

namespace rend {
    namespace ui {
        class CrossHairRenderer{
        public:
            void init();
            void render();
            void terminate();

            //! Resets the vertices so that the cursor fits to the viewport
            void update(int w, int h);
        private:
            bgfx::VertexBufferHandle m_vbh;
            bgfx::IndexBufferHandle  m_ibh;

            Texture m_texture;
            bgfx::UniformHandle m_texSampler;
            bgfx::ProgramHandle m_program;

            // THIS IS FUCKING STUPID IT DOES NOT WORK AAAAAAAAA
            QuadTexVertex m_vertexData[4] = {
                    {glm::vec3{0}, glm::vec2{0} },
                    {glm::vec3{0}, glm::vec2{0} },
                    {glm::vec3{0}, glm::vec2{0} },
                    {glm::vec3{0}, glm::vec2{0} },
            };
        };
    } // ui
} // rend

#endif //COOL_GAME_CROSSHAIR_RENDERER_HPP
