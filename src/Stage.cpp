#include "Stage.hpp"

Stage *Stage::next() const {
    return _next;
}

bool Stage::finished() const {
    return _finished;
}

void Stage::finish(Stage *next) {
    _finished = true;
    _next = next;
}
