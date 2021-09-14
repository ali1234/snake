#pragma once

#include <cstdint>
#include <memory>

class Stage;

typedef std::shared_ptr<Stage> StagePtr;

class Stage : public std::enable_shared_from_this<Stage> {
    public:
        virtual ~Stage() = default;

        virtual void update(uint32_t time) = 0;
        virtual void render(uint32_t time) = 0;

        StagePtr next();
        bool finished() const;
        bool changed() const;

    protected:
        void finish(StagePtr next);
        void change(StagePtr next);

    private:
        bool _finished = false;
        bool _changed = false;
        StagePtr _next = nullptr;
};

class Timed : public Stage {
    public:
        Timed(uint32_t duration) : Stage(), duration(duration) {};

        void update(uint32_t time) override;

    protected:
        bool expired() const;
        float progress() const;

        uint32_t duration, elapsed = 0;
};