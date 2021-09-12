//
// Created by al on 12/09/2021.
//

#include "StagePlaying.hpp"
#include "StagePause.hpp"
#include "StageTitle.hpp"

void StagePlaying::update(uint32_t time) {
    if (!finished()) {
        if (snake.game_over) {
            if (blit::buttons.pressed & blit::Button::A) {
                change(std::make_shared<StageTitle>());
            }
        } else {
            if (blit::buttons.pressed & blit::Button::MENU) {
                snake.timer.pause();
                change(std::make_shared<StagePause>(shared_from_this()));
            } else {
                if (snake.timer.is_paused())
                    snake.timer.start();
                snake.update(time);
            }
        }
    }
}

void StagePlaying::render(uint32_t time) {
    snake.render(time);
    if (snake.game_over) {
        blit::screen.pen = blit::Pen(0, 0, 0, 127);
        blit::screen.clear();
        blit::screen.pen = blit::Pen(255, 255, 255);
        blit::screen.text("Game Over\n\nPress A", blit::minimal_font, {80, 60}, true, blit::TextAlign::center_center);
    }
}
