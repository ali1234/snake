#include <string>
#include <cstring>
#include <memory>
#include <cstdlib>

#include "snake.hpp"
#include "random.hpp"

using namespace blit;

// We're using a 40 x 30 game world, making our "pixels" 8x8 actual pixels in size
// this is coincidentally the size of a tile, lest we want to use graphics in future.
constexpr Size surface_size(40, 30);
// Create the 2D surface data as type Pen so we can poke it directly if we want!
Pen surface_data[surface_size.h][surface_size.w];
// Create the 40 x 30 pixel game world canvas to draw into
Surface surface((uint8_t *)surface_data, PixelFormat::RGBA, surface_size);

// Some handy constants for movement directions and "stopped" state
constexpr Vec2 UP(0, -1);
constexpr Vec2 DOWN(0, 1);
constexpr Vec2 LEFT(-1, 0);
constexpr Vec2 RIGHT(1, 0);
constexpr Vec2 STOP(0, 0);

// Current snake direction
Vec2 direction(0, 0);

// The snake itself is a vector of Points
std::vector<Point> snake;

// Only one apple at a time, represented by a Point
Point apple;

// First to 2^32-1 points wins!
uint32_t score;

// The "update" hook is called far too quickly, so use a timer for movement
Timer timer;

// You lost. Or maybe just began.
// Reset the "snake" and all sundry variables back to their default state
void whoops() {
    snake.erase(snake.begin(), snake.end());
    snake.emplace_back(Point(19, 14));
    direction = STOP;
    score = 0;
    random_reset();
    apple = Point(14, 19);
}

// This function is called by the timer
void move(Timer &t) {
    Point head = snake.back() + direction;
    for(auto segment : snake) {
        if(head == segment){
            whoops();
            return;
        }
    }
    snake.emplace_back(head);

    if(snake.back().x > surface_size.w - 1) return whoops();
    if(snake.back().y > surface_size.h - 1) return whoops();
    if(snake.back().x < 0) return whoops();
    if(snake.back().y < 0) return whoops();

    if(snake.back() == apple) {
        score += 1;
        apple = get_random_point(surface_size);
    } else {
        snake.erase(snake.begin());
    }
}

/* setup */
void init() {
    snake.reserve(100);
    whoops();
    timer.init(move, 100, -1);
    timer.start();
}

void render(uint32_t time_ms) {
    // Draw into our virtual 40 x 30 pixel game canvas
    surface.pen = Pen(0, 20, 0);
    surface.clear();
    surface.pen = Pen(0, 255, 0);
    for(auto segment : snake) {
        surface.pixel(segment);
    }
    surface.pen = Pen(255, 0, 0);
    surface.pixel(apple);

    // *Real* screen drawing
    screen.alpha = 40;
    screen.stretch_blit(&surface, Rect(Point(0, 0), surface.bounds), Rect(Point(0, 0), screen.bounds));
    
    // Score overlay
    screen.alpha = 100;
    screen.pen = Pen(255, 255, 255);
    screen.text(std::to_string(score), minimal_font, Point(0, 0), false);
}

void update(uint32_t time) {
    // Movement is easy. You can't go back on yourself.
    if(direction.x == 0) {
        if(buttons.pressed & DPAD_RIGHT) direction = RIGHT;
        if(buttons.pressed & DPAD_LEFT) direction = LEFT;
    }
    if(direction.y == 0) {
        if(buttons.pressed & DPAD_DOWN) direction = DOWN;
        if(buttons.pressed & DPAD_UP) direction = UP;
    }
}
