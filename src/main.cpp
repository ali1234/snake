#include "Stage.hpp"
#include "StagePlaying.hpp"


Stage *stage = nullptr;

void init() {
    set_screen_mode(blit::ScreenMode::lores);
    stage = new StagePlaying();
}

void update(uint32_t time) {
    stage->update(time);
}

void render(uint32_t time) {
    stage->render(time);
}