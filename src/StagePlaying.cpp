//
// Created by al on 12/09/2021.
//

#include "StagePlaying.hpp"
#include "StagePause.hpp"

void StagePlaying::update(uint32_t time) {
    if (!finished()) {
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

void StagePlaying::render(uint32_t time) {
    snake.render(time);
}
