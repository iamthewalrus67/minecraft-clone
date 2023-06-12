//
// Created by adeon on 6/12/23.
//

#include "ui_renderer.hpp"

namespace rend {
    namespace ui {
        void UIRenderer::init() {
            m_crosshairRenderer.init();
        }

        void UIRenderer::render() {
            m_crosshairRenderer.render();
        }

        void UIRenderer::update(int w, int h) {
            m_crosshairRenderer.update(w, h);
        }

        void UIRenderer::terminate() {
            m_crosshairRenderer.terminate();
        }
    } // ui
} // rend