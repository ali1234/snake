#include "Stage.hpp"

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