//
// Created by al on 12/09/2021.
//

#include "StagePlaying.hpp"

void StagePlaying::update(uint32_t time) {
    snake.update(time);
}

void StagePlaying::render(uint32_t time) {
    snake.render(time);
}
