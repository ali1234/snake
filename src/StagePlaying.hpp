#pragma once

#include "Stage.hpp"
#include "Snake.hpp"

class StagePlaying : public Stage {
    public:
        void update(uint32_t time) override;
        void render(uint32_t time) override;
    protected:
        Snake snake;
};
