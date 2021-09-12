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
