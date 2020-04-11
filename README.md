Code Dream
==========

Animated source code... for backgrounds and stuff.

Specifically, I made this for fun to use for a Zoom virtual
background. But I'm sure there could be other uses.

![Sample](/sample.gif)

Dependencies
------------

  - gcc
  - Emacs
  - SDL2
  - SDL2_ttf
  - libgif

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

Set resolution.

    src/code-dream --width 800 --height 600 main.c

Set position.

    src/code-dream -x 0 -y 0 main.c

Specify fullscreen.

    src/code-dream -f main.c

Generate an animated GIF.

    src/code-dream -o sample.gif main.c

TODO
----

* Put the highlight information in a temporary directory or get the
  output in memory somehow instead of littering the actual source code
  directory with .txt files

* Allow using arbitrary font choices, possibly installed on the system

* Be able to run from other locations than the source directory

    * Properly search for highlight.el and the font from the program
      directory instead of assuming it's in the working directory

* Support a proper install (via `make install`) so that highlight.el
  goes in /usr/share (or equivalent) and code-dream knows where it is

* Support proper list of file suffixes for directory searching instead
  of just .c, .h, and .java

* Optimize loading of code
    * Possibly make the emacs script run on all found code files at once
    * Possibly replace the emacs script somehow

* Show the loading screen while code is loading, not just while images
  are being generated. (The reason the loading screen was originally
  for that was because that part used to be more inefficient)

* Configuration options for zoom level, maximum distance, speed, etc.

* Make it not zoom to empty space so much (know how big the line is
  and zoom to the middle of the line)

* Don't move randomly
    * Random is too random and can do the same spot too
      often. Instead, jump through the source in some regular pattern
      so that a variety is seen

* ~~Allow setting generated GIF file name~~

* Allow the user to specify a theme
    * Currently the theme used is `'tango-dark` and the colors themselves
      are hard-coded into the program in various places.
    * If we get these colors from `emacs` instead in the script then we
      can allow the user to specify a `--theme` and pass that along to
      emacs.

* Try different methods or creating GIF color maps:
    * Collect stats of colors used in every frame
        * Initialize a `48M * sizeof(long)` array indexed by color value
          (`0x000000` to `0xFFFFFF`) and increment the value for every pixel
          for every frame
        * Save frames in memory until the end
        * At the end, generate the global color map according to the most
          used colors and find closest color each pixel for each frame.
    * Try doing the same as the above but for each frame and use a
      color map per frame.

* ~~Fix GIF color maps to use whatever colors it needs instead of using
  grayscale~~

* Make a way to export to .mov directly so that I don't need to screen
  record to actually get the background video

* Use a more efficient structure for `code_image_set_t` so that it can fetch
  requested images (by character and color) without having to scan through
  all of them.

* ~~Make a way to export to animated .gif~~

* ~~Ability to use multiple files and possibly specify directories to use~~

* More interesting effects than the character jiggling

    * ~~The next piece of code in the distance of the current one,
      blurred as if coming into focus as we zoom past the current one~~
