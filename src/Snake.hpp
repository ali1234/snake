#pragma once

#include "32blit.hpp"

class Snake {
    public:
        Snake();
        void update(uint32_t time);
        void render(uint32_t time);
    protected:
        void restart_game();
        void move(blit::Timer &t);

        // Time, in milliseconds, between each update of the game state
        static constexpr uint32_t UPDATE_RATE = 200;

        // Some handy constants for movement directions and "stopped" state
        static constexpr blit::Vec2 MOVE_UP = blit::Vec2(0, -1);
        static constexpr blit::Vec2 MOVE_DOWN = blit::Vec2(0, 1);
        static constexpr blit::Vec2 MOVE_LEFT = blit::Vec2(-1, 0);
        static constexpr blit::Vec2 MOVE_RIGHT = blit::Vec2(1, 0);
        static constexpr blit::Vec2 STOP = blit::Vec2(0, 0);

        // These points index into the spritesheet, using x/y values
        // makes it easy to identify a specific sprite.
        static constexpr blit::Point SPRITE_SNAKE = {9, 10};
        static constexpr uint32_t NUM_SPRITES = 14; // First 14 sprites on the top row

        // Rectangle to store the playing field bounds in tiles
        // the screen bounds might be 160x120 but our actual
        // play area is 20x15 (8px tiles)
        blit::Rect game_bounds;

        // Current snake direction
        blit::Vec2 direction = blit::Vec2(0, 0);

        // The snake itself is a vector of Points
        std::vector<blit::Point> snake;

        // Only one apple at a time, represented by a Point
        blit::Point apple;

        // Sprite to use for the apple
        blit::Point sprite_apple = {0, 0};

        // First to 2^32-1 points wins!
        uint32_t score = 0;

        // The "update" hook is called far too quickly, so use a timer for movement
        blit::Timer timer;
};