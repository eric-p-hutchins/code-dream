Code Dream
==========

Animated source code... for backgrounds and stuff.

Specifically, I made this for fun to use for a Zoom virtual
background. But I'm sure there could be other uses.

![Sample](/sample.gif)

Runtime Dependencies
--------------------

  - Emacs

Build Dependencies
------------------

  - gcc
  - SDL2
  - SDL2_ttf

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

Run for a directory.

    src/code-dream .

Set resolution.

    src/code-dream -w 800 -h 600 main.c

Set position.

    src/code-dream -x 0 -y 0 main.c

Specify fullscreen.

    src/code-dream -f main.c

TODO
----

* Be able to run from other locations than the source directory
    * Properly search for highlight.el from the program directory instead of assuming it's in the working directory
* Support a proper install (via `make install`) so that highlight.el goes in /usr/share (or equivalent) and code-dream knows where it is
* Support proper list of file suffixes for directory searching instead of just .c, .h, and .java
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
* Make a way to export to .mov directly so that I don't need to screen
  record to actually get the background video
* ~~Ability to use multiple files and possibly specify directories to use~~
* More interesting effects than the character jiggling
    * ~~The next piece of code in the distance of the current one,
      blurred as if coming into focus as we zoom past the current one~~
