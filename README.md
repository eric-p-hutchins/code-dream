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

Generate an animated GIF (currently in grayscale and always called `test.gif`).

    src/code-dream -g main.c

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

* Configuration options for zoom level, maximum distance, speed, etc.

* Make it not zoom to empty space so much (know how big the line is
  and zoom to the middle of the line)

* Don't move randomly
    * Random is too random and can do the same spot too
      often. Instead, jump through the source in some regular pattern
      so that a variety is seen

* Allow setting generated GIF file name

* Fix GIF color maps to use whatever colors it needs instead of using
  grayscale

* Make a way to export to .mov directly so that I don't need to screen
  record to actually get the background video

* ~~Make a way to export to animated .gif~~

* ~~Ability to use multiple files and possibly specify directories to use~~

* More interesting effects than the character jiggling

    * ~~The next piece of code in the distance of the current one,
      blurred as if coming into focus as we zoom past the current one~~
