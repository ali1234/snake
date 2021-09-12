#include "Snake.hpp"

Snake *snake = nullptr;

void init() {
    set_screen_mode(blit::ScreenMode::lores);
    snake = new Snake();
}

void update(uint32_t time) {
    snake->update(time);
}

void render(uint32_t time) {
    snake->render(time);
}