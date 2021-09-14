#include "32blit.hpp"

#include "StageAttract.hpp"
#include "StageTitle.hpp"

void StageAttract::update(uint32_t time) {
    Timed::update(time);
    if (!finished()) {
        if (expired() || blit::buttons.pressed & blit::Button::A)
            finish(std::make_shared<StageTitle>());
    }
}

void StageAttract::render(uint32_t time) {
    snake.render(time);
    blit::screen.pen = blit::Pen(255, 255, 255);
    blit::screen.text("Attract Mode", blit::minimal_font, {80, 119}, true, blit::TextAlign::bottom_center);
}
