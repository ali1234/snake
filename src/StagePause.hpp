#pragma once

#include "Stage.hpp"

class StagePause : public Stage {
    public:
        StagePause(Stage *background) : background(background) {}
        void update(uint32_t time) override;
        void render(uint32_t time) override;
    protected:
        Stage *background;
};