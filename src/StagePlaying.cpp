//
// Created by al on 12/09/2021.
//

#include "StagePlaying.hpp"
#include "StagePause.hpp"

void StagePlaying::update(uint32_t time) {
    if (!finished()) {
        if (blit::buttons.pressed & blit::Button::MENU)
            change(std::make_shared<StagePause>(shared_from_this()));
        else
            snake.update(time);
    }
}

void StagePlaying::render(uint32_t time) {
    snake.render(time);
}
