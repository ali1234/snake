#include "Stage.hpp"
#include "StageTitle.hpp"


static StagePtr stage = nullptr;

uint32_t prev_time = 0;

void init() {
    set_screen_mode(blit::ScreenMode::lores);
    stage = std::make_shared<StageTitle>();
    prev_time = blit::now();
}

void update(uint32_t time) {
    uint32_t delta = time - prev_time;
    if (delta > 100) delta = 100;
    if (delta > 0) {
        stage->update(delta);
        if (stage->changed()) stage = stage->next();
        prev_time = time;
    }
}

void render(uint32_t time) {
    stage->render(time);
}