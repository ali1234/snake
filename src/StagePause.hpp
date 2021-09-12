#pragma once

#include "Stage.hpp"

class StagePause : public Stage {
    public:
        StagePause(StagePtr background) : background(background) {}
        void update(uint32_t time) override;
        void render(uint32_t time) override;
    protected:
        StagePtr background;
};