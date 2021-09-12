#include "Stage.hpp"
#include "StageTitle.hpp"


static StagePtr stage = nullptr;

void init() {
    set_screen_mode(blit::ScreenMode::lores);
    stage = std::make_shared<StageTitle>();
}

void update(uint32_t time) {
    stage->update(time);
    if (stage->changed()) stage = stage->next();
}

void render(uint32_t time) {
    stage->render(time);
}