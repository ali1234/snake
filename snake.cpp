#include <string>
#include <cstring>
#include <memory>
#include <cstdlib>

#include "snake.hpp"
#include "random.hpp"
#include "assets.hpp"

using namespace blit;

// Some handy constants for movement directions and "stopped" state
constexpr Vec2 move_up(0, -1);
constexpr Vec2 move_down(0, 1);
constexpr Vec2 move_left(-1, 0);
constexpr Vec2 move_right(1, 0);
constexpr Vec2 STOP(0, 0);

// These points index into the spritesheet, using x/y values
// makes it easy to identify a specific sprite.
constexpr Point sprite_snake(9, 10);
constexpr Point sprite_apple(0, 0);

Rect game_bounds;

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
void restart_game() {
    snake.clear();
    snake.emplace_back(game_bounds.center());
    direction = STOP;
    score = 0;
    random_reset();
    apple = get_random_point(game_bounds.size());
}

// This function is called by the timer and handles the main game logic
// this allows us to run the game at a speed (more or less) of our choosing.
// TASK you could try making Snake harder over time by speeding it up!
void move(Timer &t) {
    Point head = snake.back() + direction;
    for(auto segment : snake) {
        // If the head x/y coordinates match any body/segment
        // coordinates then we've collided with ourselves. BAD LUCK!
        if(head == segment){
            restart_game();
            return;
        }
    }
    // Add the new head to our snake's body so it grows
    // in the direction of movement.
    snake.emplace_back(head);

    if(head == apple) {
        score += 1;
        apple = get_random_point(game_bounds.size());
    } else {
        //  If we go out of bounds BAD LUCK!
        // We can check this by seeing if our head is within the game bounds.
        if(!game_bounds.contains(head)) {
            restart_game();
            return;
        }
        // If we haven't eaten an apple then the snake doesn't get any bigger
        // erase the tail... this means erasing the front of our std::vector
        // which is A BAD THING, but for the sake of snake... it's fiiiinnnee!
        snake.erase(snake.begin());
    }
}

/* setup */
void init() {
    set_screen_mode(lores);
    
    // The screen bounds divided by the sprite size (8 x 8) form
    // the game bounds- ie: the number of x / y locations that
    // our snake and apple's can occupy.
    game_bounds = Rect(Point(0, 0), screen.bounds / 8);

    // Load our dingbads spritesheet, it has some... snake-like stuff we can use
    // TASK Create some artwork and make Snake your own!
    screen.sprites = Surface::load(asset_dingbads);

    snake.reserve(100);

    // Make sure the game state is reset
    restart_game();

    // Set up and start the timer
    timer.init(move, 100, -1);
    timer.start();
}

void render(uint32_t time_ms) {
    // Clear the screen to dark green
    screen.pen = Pen(0, 20, 0, 255);
    screen.clear();

    // Draw the snake body one segment at a time
    for(auto segment : snake) {
        // To convert from segment coords to screen coords we must multiply
        // by the sprite size: 8 x 8
        screen.sprite(sprite_snake, segment * 8);
    }

    // Draw the current apple. Or cherry. Or something.
    screen.sprite(sprite_apple, apple * 8);

    // Draw a score overlay
    screen.alpha = 100;
    screen.pen = Pen(255, 255, 255);
    screen.text(std::to_string(score), minimal_font, Point(5, 5), false);
}

void update(uint32_t time) {
    // Movement is easy. You can't go back on yourself.
    if(direction.x == 0) {
        if(buttons.pressed & DPAD_RIGHT) direction = move_right;
        if(buttons.pressed & DPAD_LEFT) direction = move_left;
    }
    if(direction.y == 0) {
        if(buttons.pressed & DPAD_DOWN) direction = move_down;
        if(buttons.pressed & DPAD_UP) direction = move_up;
    }
}
