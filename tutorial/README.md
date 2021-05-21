# 32Blit Snake, from brass tacks

I chose the game Snake around which to build this tutorial in the hope that everyone has at least a passing familiarity with the game, it mechanics and goals.

Fo those that don't, Snake is simple. You are a snake. Your objective is to wind your way around a level and eat apples. Every apple eaten makes you longer. If your head collides with any part of your body, you lose.

The beauty of snake is its simplicity. It's a prime example of a first-game-to-grok because it has only one moving part; you, the player, the titular Snake.

This tutorial will guide you through:

1. How not to do graphics
2. Creating and moving the player Snake
3. Setting the limits of the playing field
4. Spawning random apples
5. Eating apples, and growing the snake
6. Bumping the snake into it's own curly bottom

## Graphics - or how not to do them

To keep this tutorial simple we're not going to deal with sprites or fancy graphics. We're going back to the classic Nokia snake era and dealing only with a handful of pixels to represent the Snake and the apples to collect.

In order to do this, we're going to draw the entire game into its own Surface. A Surface is, effectively, an image and we're playing a programming game of realtime MS Paint.

Our Surface will be 40 by 30 pixels in size. That's pretty small, but there's a good reason for this choice. A 32Blit sprite is 8x8 pixels, and 40 x 30 is 32Blit's screen resolution (320 x 240) divided by 8 in both directions. If we decide we want glorious sprite-based after all, we can make a few changes to our drawing code and - voila - everything will just work.

### The Code

We know a few key things that will get us started:

1. We want to draw into a Surface
2. So we're going to need a Surface
3. We want it to be 40 x 30 pixels in size
4. So we'll need to stretch it to fill the screen

#### Creating A Surface

C++ code can get pretty obtuse pretty quickly so I'm going to keep it brief. These lines of code are... *a* way to create a 40 x 30 pixel, RGBA (red, green, blue + alpha) surface into which we can draw our game world.

```c++
constexpr Size surface_size(40, 30);
Pen surface_data[surface_size.h][surface_size.w];
Surface surface((uint8_t *)surface_data, PixelFormat::RGBA, surface_size);
```

To break this down, first we're defining a `Size` which is nothing more than a fancy container to store a width `.w` and height `.h` value together, representing the two-dimensional size of *something*. That *something* can be anything you can imagine, but for our purposes it's our surface.

The line:

```c++
constexpr Size surface_size(40, 30);
```

Is a little about-face, but it creates a constant value named `surface_size` with a `.w` of 40 and a `.h` of 30.

You could accomplish the same thing with the following snippet, but I like brevity and we're going to be writing quite a bit of code here:

```c++
constexpr Size surface_size = Size(40, 30);
```

The next line is a little less weird:

```c++
Pen surface_data[surface_size.h][surface_size.w];
```

This creates a 2D array. A 2D array of `Pen`s. A 2D array of `Pen`s big enough to store a `Pen` for every pixel in our 40 x 20 Surface. Since a `Pen` represents a colour - Red, Green, Blue + Alpha - an 2D array of `Pen`s is effectively an image. Indeed this forms the very foundation of a Surface, and our `Surface` is really just a convenience wrapper around this array to define the real 2D size of our image and provide drawing and blending functions.

Now, lucky us, we get to create the Surface itself:

```c++
Surface surface((uint8_t *)surface_data, PixelFormat::RGBA, surface_size);
```

The `Surface` takes three arguments. Some kind of storage, a "Pixel Format" and the `Size` we created earlier. The latter two arguments serve to describe the storage backing our `Surface` and since this can be anything from a `Pen` (for RGBA) down to a `uint8_t` (for masks and paletted images) the `Surface` actually expects an array of `uint8_t`.

Woah there. What does that mean?

Well `u` `int` `8` `_t` roughly translates to:

* `unsigned` - it's a positive number only from 0 upwards
* `integer` - a whole number, no fuzzy decimal places
* `8 bits` - 8 bits of resolution- fitting a number from 0 to 255
* `_t` - this is a type, a type of number in this case... this suffix is just visual noise but don't forget it.

Since a `uint8_t` is usually the smallest data type, we tend to create functions that take a `uint8_t` and use fancy C features to tell our compiler to treat blocks of data as one thing or another.

Since a `Pen` stores a Red, Green, Blue and Alpha value, all as unsigned, 8-bit integers, it can be represented as a 32-bit integer, or four 8-bit integers, or two 16-bit integers. The `Surface` constructor expects a container that's made of `uint8_t` though, so we just tell the compiler to treat our array of `Pen`s as an array of `uint8_t` and it's happy to oblige.

We could have used `uint8_t` in the first place, but there's a good reason for `surface_data` being made of `Pen`s that we'll come to later.

#### Drawing Something Into Our Surface

Before we forge ahead and draw our `Surface` to the screen, we ought to draw something in the Surface first. Why? Well if we don't put anything in there, we're not going to know if drawing it to the screen has worked. There will be nothing to see!

Let's poke something pretty to look at:

```c++
void init() {
    surface_data[14][19] = Pen(255, 0, 0);
}
```

Okay, okay. We're cheating a bit here. By poking a `Pen` value directly into our `surface_data` we're avoiding the fancy, helpful drawing functions that Surface affords us. We'll get to that later.

Isn't it cool that we can just poke colours right into our display, though? Be forewarned, however, that due to the way we tend to represent pixel data the *first* index in our 2D array is the *vertical* axis (y) and the *second* index is the *horizontal* axis (x). This is so horiontal rows of pixels are stored ajacent to each other, which has lots of benefits I'm not going to touch upon just yet.

#### Drawing Our Surface To The Screen

Now we have one dot roughly in the center of our surface, we want to stick it on our screen so we can *see* it:

```c++
void render(uint32_t time_ms) {
  screen.stretch_blit(&surface, Rect(Point(0, 0), surface.bounds), Rect(Point(0, 0), screen.bounds));
}
```

This is a little wordy, but hopefully you'll notice the to `Rect()`s are rather similar. The first is just a rectangle representing our surface. The second is a rectangle representing our screen. These allow `stretch_blit` to know how much of our surface it should grab and which portion of the screen to paint it onto.

Since `Rect` can take a `Point` and a `Size`, we borrow the `.bounds` of our surface and screen (already a `Size`) and use `Point(0, 0)` to represent the top left corner of each.

If you wanted to draw to a quarter of the screen, you could just divide its `.bounds` by 2:

```c++
void render(uint32_t time_ms) {
  screen.stretch_blit(&surface, Rect(Point(0, 0), surface.bounds), Rect(Point(0, 0), screen.bounds / 2));
}
```

Four player snake? You might need more buttons!

That concludes this second, by now your code should look like the snippet below and you should be:

1. Creating a surface that represents the canvas into which we paint our pixelly Snake art
2. Painting something into the surface so we know it works
3. Drawing the surface to the screen

```c++
constexpr Size surface_size(40, 30);
Pen surface_data[surface_size.h][surface_size.w];
Surface surface((uint8_t *)surface_data, PixelFormat::RGBA, surface_size);

void init() {
    surface_data[14][19] = Pen(255, 0, 0);
    // Note! You should really do this:
    // surface.pen = Pen(255, 0, 0)
    // surface.pixel(Point(19, 14))
    // but... we're lazy and lo-fi in this here town!
}

void render(uint32_t time_ms) {
  screen.stretch_blit(&surface, Rect(Point(0, 0), surface.bounds), Rect(Point(0, 0), screen.bounds / 2));
}
```