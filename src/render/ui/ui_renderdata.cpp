//
// Created by adeon on 6/12/23.
//

#include "ui_renderdata.hpp"

namespace rend {
    namespace ui {
        bgfx::VertexLayout QuadTexVertex::layout;

        void QuadTexVertex::init() {
            layout
                    .begin()
                    .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
                    .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
                    .end();
        }
    } // ui
} // rend