#include "Snake.hpp"

Snake *snake = nullptr;

void init() {
    snake = new Snake();
}

void update(uint32_t time) {
    snake->update(time);
}

void render(uint32_t time) {
    snake->render(time);
}