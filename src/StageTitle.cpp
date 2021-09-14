#include "32blit.hpp"

#include "StageTitle.hpp"
#include "StageAttract.hpp"
#include "StagePlaying.hpp"

void StageTitle::update(uint32_t time) {
    Timed::update(time);
    if (!finished()) {
        if (expired())
            finish(std::make_shared<Fade>(shared_from_this(), std::make_shared<StageAttract>()));
        if (blit::buttons.pressed & blit::Button::A)
            finish(std::make_shared<Fade>(shared_from_this(), std::make_shared<StagePlaying>()));
    }
}

void StageTitle::render(uint32_t time) {
    blit::screen.pen = blit::Pen(255, 127, 0);
    blit::screen.clear();
    blit::screen.pen = blit::Pen(255, 255, 255);
    blit::screen.text("Snake", blit::minimal_font, {80, 60}, true, blit::TextAlign::center_center);
}
