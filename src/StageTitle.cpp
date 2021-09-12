#include "32blit.hpp"

#include "StageTitle.hpp"

void StageTitle::update(uint32_t time) {

}

void StageTitle::render(uint32_t time) {
    blit::screen.pen = blit::Pen(255, 127, 0);
    blit::screen.clear();
    blit::screen.pen = blit::Pen(255, 255, 255);
    blit::screen.text("Snake", blit::minimal_font, {80, 60}, true, blit::TextAlign::center_center);
}
