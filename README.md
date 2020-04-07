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
  - SDL

Build
-----
    make

Run
---
    ./code-dream

TODO
----

* Configuration options for zoom level, maximum distance, speed, etc.
* Ability to use multiple files and possibly specify directories to use
* Make it not zoom to empty space so much (know how big the line is
  and zoom to the middle of the line)
* Don't move randomly
    * Random is too random and can do the same spot too
      often. Instead, jump through the source in some regular pattern
      so that a variety is seen
* Make a way to export to .mov directly so that I don't need to screen
  record to actually get the background video
* More interesting effects than the character jiggling
    * The next piece of code in the distance of the current one,
      blurred as if coming into focus as we zoom past the current one
