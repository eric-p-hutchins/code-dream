Code Dream
==========

Animated source code... for backgrounds and stuff.

Specifically, I made this for fun to use for a Zoom virtual
background. But I'm sure there could be other uses.

Dependencies
------------

  - gcc
  - Emacs
  - SDL2
  - SDL2_ttf
  - libgif
  - ffmpeg

Build
-----

If from a git clone, generate the scripts:

    autoreconf -i

Then:

    ./configure
    make

Run
---

    src/code-dream main.c

Run for all code found in a directory.

    src/code-dream .

Any files ending with the following extensions will be recognized
(chosen somewhat arbitrarily, partly based on having used it or come
across it before somewhere):

* `.as`
* `.c`
* `.c++`
* `.cc`
* `.cl`
* `.coffee`
* `.cpp`
* `.cs`
* `.css`
* `.el`
* `.go`
* `.h`
* `.hs`
* `.html`
* `.java`
* `.js`
* `.m`
* `.mm`
* `.pl`
* `.py`
* `.rb`
* `.rs`
* `.scm`
* `.sh`

Examples:

* `code-dream --width 240 --height 150 src/main.c`

    Use the default Emacs colors.

    ![Animated GIF of program source using default colors](/default.gif)

* `code-dream --width 240 --height 150 -d src/main.c`

    Use the default Emacs colors when you have a "dark" background.

    ![Animated GIF of program source using default dark colors](/default-dark.gif)

* `code-dream --width 240 --height 150 -t tango-dark src/main.c`

    Use the Emacs tango-dark theme. Any Emacs theme can be specified
    (something that would work when using `M-x load-theme` in Emacs)

    ![Animated GIF of program source using tango-dark theme](/tango-dark.gif)

* `code-dream --width 240 --height 150 -t tango-dark -o tango-dark.gif src/main.c`

    Generate an animated GIF called `tango-dark.gif` (what I ran to
    create the above GIF file)

* `code-dream --width 240 --height 150 -t tango-dark -o tango-dark.mov src/main.c`

    Generate a `.mov` video file of the animation.

TODO
----

* Don't duplicate the drawing calls, but instead, in the main file,
  split out a common draw function that uses a passed in renderer and
  then call that from the main draw and after drawing to screen, draw
  to `gif_writer` renderers or `video_writer` renderers

* Unify the gif writer and video writer objects into one that is smart
  enough to know what to do for whichever format was selected,
  possibly using ffmpeg to create the gif instead of libgif

* Write tests, specifically around parsing themes properly, but look
  for other things to verify
    * Write a little contrived emacs theme and verify that the output
      of `theme.el` matches what you expect

* See if it's possible to draw the characters by "glyph" shape that is stored
  so that pre-rendered textures don't need to be used. This may require
  ditching SDL_ttf and instead using freetype or something like that directly

* Investigate streaming the GIF data to the file somehow so that it
  doesn't have to keep it all in memory until it is written. At this
  point it takes a lot of memory to write the GIF because until the
  program exits and we run `EGIFSpew` it keeps all the stored images
  in memory and they can take up a lot of space. Maybe using the
  lower-level API instead of EGifSpew, we can free the stored images
  as we go?

* Try different methods of creating GIF color maps:
    * Collect stats of colors used in every frame
        * Initialize a `48M * sizeof(long)` array indexed by color value
          (`0x000000` to `0xFFFFFF`) and increment the value for every pixel
          for every frame
        * Save frames in memory until the end
        * At the end, generate the global color map according to the most
          used colors and find closest color each pixel for each frame.
    * Try doing the same as the above but for each frame and use a
      color map per frame.
        * This will not use as much memory since it doesn't need to save the
          frames (unlike when trying to make a global color map) but the
          resulting file will be bigger (because it needs to write a color
          map for each frame)

* Allow using arbitrary font choices, possibly installed on the system

* Optimize loading of code
    * Possibly make `highlight.el` run on all found code files at once
    * Possibly replace `highlight.el` somehow

* Configuration options for zoom level, maximum distance, speed, etc.

* Make it not zoom to empty space so much (know how big the line is
  and zoom to the middle of the line)

* Don't move randomly
    * Random is too random and can do the same spot too
      often. Instead, jump through the source in some regular pattern
      so that a variety is seen

* Use a more efficient structure for `code_image_set_t` so that it can
  fetch requested images (by character, color, and weight) without
  having to scan through all of them.

* Do something different for different levels of bold specified by
  Emacs face spec: `(semi-bold bold extra-bold ultra-bold)`

* ~~Make a way to export to .mov directly so that I don't need to screen
  record to actually get the background video~~
    * ~~I'm thinking ffmpeg has a library for doing something like that~~

* ~~In the fading-in effect, don't just make it translucent but merge
  the color with the background color (still at full opacity) before
  drawing so that it blocks what's behind it, like fog~~

* ~~Respect bold attribute in themes.~~

* ~~Allow the user to specify a theme~~
    * ~~Currently the theme used is `'tango-dark` and the colors themselves
      are hard-coded into the program in various places.~~
    * ~~If we get these colors from `emacs` instead in the script then we
      can allow the user to specify a `--theme` and pass that along to
      emacs.~~

* ~~Support proper list of file suffixes for directory searching instead
  of just .c, .h, and .java~~

* ~~Support a proper install (via `make install`) so that highlight.el
  goes in /usr/share (or equivalent) and code-dream knows where it is~~

* ~~Put the highlight information in a temporary directory or get the
  output in memory somehow instead of littering the actual source code
  directory with .txt files~~

* ~~Be able to run from other locations than the source directory~~

    * ~~Properly search for highlight.el and the font from the program
      directory instead of assuming it's in the working directory~~

* ~~Show the loading screen while code is loading, not just while images
  are being generated. (The reason the loading screen was originally
  for that was because that part used to be more inefficient)~~

* ~~Allow setting generated GIF file name~~

* ~~Fix GIF color maps to use whatever colors it needs instead of using
  grayscale~~

* ~~Make a way to export to animated .gif~~

* ~~Ability to use multiple files and possibly specify directories to use~~

* More interesting effects than the character jiggling

    * ~~The next piece of code in the distance of the current one,
      blurred as if coming into focus as we zoom past the current one~~
