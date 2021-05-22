# Super Simple Snake

If you've somehow not heard of Snake, it's a game with a simple premise and basic controls. Inspired by classic computer games with a similar premise, it was developed for prehistoric mobile phones that didn't have fancy things like d-pads or high-resolution screens. You are a snake. Your goal is to move around the playing field, eat apples and grow. As you grow longer you're met with the challenge of avoiding your own tail. Collide with your tail, or the edge of the playing field, and it's game over.

Snake - which needed only directional controls - leant itself supremely well to the Nokia 6110 for which it was written. With the 4-way nav buttons being somewhat awkward the number keys 2, 4, 6 and 8 were used. Controlling the game was still a challenge, one that modern d-pads have mitigated somewhat, but the simple gameplay loop and ability to restart immediately to try, try again made it addictive and ultimately satisfying.

This tutorial will guide you through the 100 or so lines of code needed to crete a very, very simple Snake for the 32Blit. Super Simple Snake, if you will. It will touch upon seven key topics:

1. Displaying sprite-based graphics
2. Accepting and responding to user input
3. Using a Timer to advance game state
4. Randomly placing and eating apples
5. Managing the length/direction of the titular Snake
6. Detecting collision with the Snake's tail and the screen edge
7. Resetting the game state

These will be covered roughly in-order, and we'll rig up some test code that will later be re-written as we move toward a final Snake clone.

- [Super Simple Snake](#super-simple-snake)
  - [Step 1: Graphics](#step-1-graphics)
    - [Referencing Sprites](#referencing-sprites)
    - [Loading Sprites](#loading-sprites)
    - [Drawing Sprites](#drawing-sprites)
    - [Putting It Together](#putting-it-together)

## Step 1: Graphics

To get started quickly, we're going to use some of the assets provides with the 32Blit SDK: dingbads.

All of the 32Blit sprite assets are 128 x 128 pixel, PNG images containing 16 x 16 (256) sprites. These might form stanalone 8 x 8 characters, or be combined to form larger characters like monsters, ships and more. Dingbads is a series of 256 individual objects covering everything from fruit and food, to more abstract shapes.

### Referencing Sprites

Before we start drawing things to screen, it's important that we know how to refer to a particular sprite in our spritesheet.

Generally it's easiest to refer to sprites - each 8 x 8 pixels in size - by their index along the x and y axes. The top-left most sprite is `Point(0, 0)` and the bottom-right most is `Point(15, 15)`. Don't forget that we're counting from zero.

We can also use an index starting from `0` and ending at `255`, counting from left to right and wrapping down and left as if we were counting the words in a book. This is sometimes useful if we want to store a sprite's position in a *really small data type* to create a whole level or environment, but we'll stick with x and y coordinates for now. 32Blit can handle a little lavishness for the sake of snake.

The top-left corner in our dingbags spritesheet has a great little cherry sprite, a true classic. We'll be using this as our "apple" since it's easy to find at `Point(0, 0)`. For our snake we'll delve further into the spritesheet and use the green ball thingy on the 11th column, 10th row or `Point(10, 9)`.

Let's start writing some very tentative code to make a note of these:

```c++
using namespace blit;

constexpr Point sprite_snake(9, 10);
constexpr Point sprite_apple(0, 0);
```

First we're telling our compiler that all of the code we're about to write will be using the goodies in the 32Blit SDK. The line `using namespace blit;` allows us to drop th `blit::` qualifier before 32Blit SDK types and functions- without this we'd have to write `blit::Point` and we're *lazy*.

Then we create two points, one to store the location of our `snake` sprite, and one for the `apple`. We define these once at the top of our code so we can use `sprite_snake` and `sprite_apple` every time we refer to them. This is much easier to understand at a glance than running across `Point(0, 0)` in our code.

> ### constexpr?
> We're creating a couple of constant expressions- these are evaluated at *compile time* and are a fancy alternative to `#define SNAKE Point(9, 10)` which - depending on your experience with C/C++ you may have come across. A `constexpr Point` will create and store exactly one canonical instance of our `snake` or `apple` location in a way that the rest of our C++ code can understand. It has a type. It has an address. It's a real C++ variable. The `#define` method, by contrast, is evaluated *before* compilation by a preprocessor that doesn't undertand or care about C++. The preprocessor naively replaces every instance of `SNAKE` and `APPLE` in our code with our `Point`s.

### Loading Sprites

The 32Blit SDK and tools provide a means to turn sprites from `.png` images into chunks of data that the 32Blit engine can understand. We call it the Asset Pipeline. To accomplish this you need four things:

1. Your asset/image
2. An `assets.yml` file
3. A tweak to your `CMakeLists.txt` to add assets
4. The assets header included into your code

Since we know we're using `s4m_ur4i-dingbads.png` for our sprites, we can go right ahead and write an `assets.yml` to reference it:

```yml
assets.cpp:
  assets/s4m_ur4i-dingbads.png:
    name: asset_dingbads
```

Let's break down these lines.

```yml
assets.cpp:
```

Tells the asset pipeline to produce a `.cpp` file (and correponding `.hpp`) as its output.

```yml
  assets/s4m_ur4i-dingbads.png:
```

This is just the path to the file we want to use. The `:` on the end is important, because it tells the pipeline that the following lines are options relating to this file.

```yml
    name: asset_dingbads
```

Tells the asset pipeline that we want a variable named `asset_dingbads`.

So that's our `assets.yml`. Let's add it to `CMakeLists.txt`;

```cmake
blit_assets_yaml(${PROJECT_NAME} assets.yml)
```

Finally we can include the `.hpp` file into our code so that it knows our new asset variable exists:

```c++
#include "assets.hpp"
```

Once a spritesheet has been converted into data and embedded into our code by the asset pipeline, we need to load it and use it. This is normally done in 32Blit's `init()`:

```c++
void init() {
    screen.sprites = Surface::load(asset_dingbads);
}
```

That's a nice clear line of code. We're creating a new `Surface` (a surface is basically an image with some handy extra info and functions attached), and telling it to load the contents of `asset_dingbads`. This `Surface` is, in turn, assigned to the `screen` as its spritesheet. `screen` is *also* a surface.

> ### surface?
> The name "surface" appears a lot in computers graphics, and in terms of 32Blit it usually refers to something you *draw* into. Just like you're draw *on* a real, physical surface.
> A surface usually has a size and a pixel format - RGB, RGBA, Paletted, etc. These tidbits are encoded into our `asset_dingbads` and loaded automagically.

### Drawing Sprites

It's all been groundwork so far, and we've yet to see the fruits of our labour. Let's draw a sprite to the screen so we have something to celebrate. This is usually done in 32Blit's `render()`:

```c++
void render(uint32_t time) {
    screen.sprite(sprite_apple, Point(0, 0));
}
```

Remember the `Point` from earlier that we called `sprite_apple`? This is a reference into our spritesheet and is given as the first argument of `screen.sprite`. The second argument - you might guess - is the position on the screen where we want to draw our sprite. `Point(0, 0)` is the very top left. Perhaps you'd like to try drawing it elsewhere!

### Putting It Together

By now you should be able to load and draw a sprite. You could try drawing more sprites. Your code should look something like this:

```c++
#include "32blit.hpp"
#include "assets.hpp"

using namespace blit;

constexpr Point sprite_snake(9, 10);
constexpr Point sprite_apple(0, 0);

void init() {
    screen.sprites = Surface::load(asset_dingbads);
}

void render(uint32_t time) {
    screen.sprite(sprite_apple, Point(0, 0));
}
```