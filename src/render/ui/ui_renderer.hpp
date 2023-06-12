//
// Created by adeon on 6/12/23.
//

#ifndef COOL_GAME_UI_RENDERER_HPP
#define COOL_GAME_UI_RENDERER_HPP

#include "crosshair_renderer.hpp"

namespace rend {
    namespace ui {
        class UIRenderer {
        public:
            void init();
            void render();
            void terminate();

            //! Update all the UI component viewports
            void update(int w, int h);
        private:
            CrossHairRenderer m_crosshairRenderer;
        };
    }
}

#endif //COOL_GAME_UI_RENDERER_HPP
