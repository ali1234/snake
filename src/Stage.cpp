#include "Stage.hpp"

#include "32blit.hpp"

StagePtr Stage::next() {
    _changed = false;
    StagePtr __next = _next;
    _next = nullptr;
    return __next;
}

bool Stage::finished() const {
    return _finished;
}

void Stage::finish(StagePtr next) {
    change(next);
    _finished = true;
}

void Stage::change(StagePtr next) {
    _next = next;
    _changed = true;
}

bool Stage::changed() const {
    return _changed;
}


void Timed::update(uint32_t time) {
    elapsed += time;
}

bool Timed::expired() const {
    return elapsed > duration;
}

float Timed::progress() const {
    float tmp = (float) elapsed / duration;
    if (tmp < 0) return 0;
    if (tmp > 1) return 1;
    return tmp;
}

void Fade::update(uint32_t time) {
    Timed::update(time);
    if (!finished()) {
        if (expired()) {
            if (direction) {
                finish(b);
            } else {
                direction = true;
                elapsed = 0;
            }
        }
    }
}

void Fade::render(uint32_t time) {
    if (direction) {
        b->render(time);
        int a = 255*sinf((1.0f - progress())*3.141f*0.5f);
        blit::screen.pen = blit::Pen(0, 0, 0, a);
        blit::screen.clear();
    } else {
        a->render(time);
        int a = 255*sinf(progress()*3.141f*0.5f);
        blit::screen.pen = blit::Pen(0, 0, 0, a);
        blit::screen.clear();
    }
}


