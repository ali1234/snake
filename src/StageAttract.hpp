#pragma once

#include "Stage.hpp"
#include "Snake.hpp"

class StageAttract : public Timed {
    public:
        StageAttract() : Timed(5000) {}
        void update(uint32_t time) override;
        void render(uint32_t time) override;
};