#include "32blit.hpp"

#include "StagePause.hpp"


void StagePause::update(uint32_t time) {
    if (!finished()) {
        if (blit::buttons.pressed & blit::Button::A)
            finish(background);
    }
}

void StagePause::render(uint32_t time) {
    background->render(time);
    blit::screen.pen = blit::Pen(0, 0, 0, 127);
    blit::screen.clear();
    blit::screen.pen = blit::Pen(255, 255, 255);
    blit::screen.text("Paused", blit::minimal_font, {80, 60}, true, blit::TextAlign::center_center);
}
