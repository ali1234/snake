#include <string>
#include <cstring>
#include <memory>
#include <cstdlib>

#include "snake.hpp"
#include "random.hpp"
#include "assets.hpp"

using namespace blit;

// Time, in milliseconds, between each update of the game state
const uint32_t UPDATE_RATE = 200;

// Some handy constants for movement directions and "stopped" state
enum Direction {
    Up, Down, Left, Right, Stop
};

// These points index into the spritesheet, using x/y values
// makes it easy to identify a specific sprite.
const Point SPRITE_SNAKE(9, 10);
const uint32_t NUM_SPRITES = 14; // First 14 sprites on the top row

// Rectangle to store the playing field bounds in tiles
// the screen bounds might be 160x120 but our actual
// play area is 20x15 (8px tiles)
Rect game_bounds;

// Current snake direction
Direction direction = Stop;
Direction last_move = Stop;

// The snake itself is a vector of Points
std::vector<Point> snake;

// Only one apple at a time, represented by a Point
Point apple;

// Sprite to use for the apple
Point sprite_apple(0, 0);

// First to 2^32-1 points wins!
uint32_t score;

// The "update" hook is called far too quickly, so use a timer for movement
Timer timer;

// You lost. Or maybe just began.
// Reset the "snake" and all sundry variables back to their default state
void restart_game() {
    // Remove all snake 
    snake.clear();
    snake.emplace_back(game_bounds.center());
    direction = Stop;
    score = 0;
    random_reset();
    apple = get_random_point(game_bounds.size());
    sprite_apple.x = get_random_int() % NUM_SPRITES;
}

// This function is called by the timer and handles the main game logic
// this allows us to run the game at a speed (more or less) of our choosing.
// TASK you could try making Snake harder over time by speeding it up!
void move(Timer &t) {

    Point head = snake.back();
    switch(direction) {
        case Up:
            head.y -= 1;
            break;
        case Down:
            head.y += 1;
            break;
        case Left:
            head.x -= 1;
            break;
        case Right:
            head.x += 1;
            break;
        default:
            // No movement means nothing to do here
            // exit early so the snake doesn't continually collide with itself
            return;
    }

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

    // If the head "Point" equals the apple "Point" then Susi has
    // eaten the apple. Bump the score and spawn a new apple.
    if(head == apple) {
        score += 1;
        apple = get_random_point(game_bounds.size());
        sprite_apple.x = get_random_int() % NUM_SPRITES;
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
    last_move = direction;
}

void init() {
    set_screen_mode(lores);
    
    // The screen bounds divided by the sprite size (8 x 8) form
    // the game bounds- ie: the number of x / y locations that
    // our snake and apple's can occupy.
    game_bounds = Rect(Point(0, 0), screen.bounds / 8);

    // Load our dingbads spritesheet, it has some... snake-like stuff we can use
    // TASK Create some artwork and make Snake your own!
    screen.sprites = Surface::load(asset_dingbads);

    // Reserve enough memory for a snake that fills the entire screen
    // At 160x120 this would be 20x15 tiles, or 300 segments.
    snake.reserve(game_bounds.size().area());

    // Make sure the game state is reset
    restart_game();

    // Set up and start the timer
    timer.init(move, UPDATE_RATE, -1);
    timer.start();
}

void render(uint32_t time_ms) {
    // Clear the screen to dark green
    screen.alpha = 100;
    screen.pen = Pen(0, 20, 0, 255);
    screen.clear();
    screen.alpha = 255;

    // Draw the snake body one segment at a time
    for(auto segment : snake) {
        // To convert from segment coords to screen coords we must multiply
        // by the sprite size: 8 x 8
        screen.sprite(SPRITE_SNAKE, segment * 8);
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
    // The last actual move made is remembered in last_move.
    // You can go in any direction except the opposite of it.
    if(buttons.pressed & DPAD_RIGHT && last_move != Left) direction = Right;
    if(buttons.pressed & DPAD_LEFT && last_move != Right) direction = Left;
    if(buttons.pressed & DPAD_DOWN && last_move != Up) direction = Down;
    if(buttons.pressed & DPAD_UP && last_move != Down) direction = Up;
}
