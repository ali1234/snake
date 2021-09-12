#pragma once

#include <cstdint>

class Stage {
    public:
        virtual void update(uint32_t time) = 0;
        virtual void render(uint32_t time) = 0;
};
