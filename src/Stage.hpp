#pragma once

#include <cstdint>

class Stage {
    public:
        virtual ~Stage() = default;

        virtual void update(uint32_t time) = 0;
        virtual void render(uint32_t time) = 0;

        Stage *next() const;
        bool finished() const;

    protected:
        void finish(Stage *next);

    private:
        bool _finished = false;
        Stage *_next = nullptr;
};
