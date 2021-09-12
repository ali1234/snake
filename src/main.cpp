#include "Stage.hpp"
#include "StageTitle.hpp"


Stage *stage = nullptr;

void init() {
    set_screen_mode(blit::ScreenMode::lores);
    stage = new StageTitle();
}

void update(uint32_t time) {
    stage->update(time);
    if (stage->finished()) {
        Stage *tmp = stage->next();
        delete stage;
        stage = tmp;
    }

}

void render(uint32_t time) {
    stage->render(time);
}