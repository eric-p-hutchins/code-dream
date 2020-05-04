// Copyright (C) 2020 Eric P. Hutchins

// This file is part of Code Dream.

// Code Dream is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// Code Dream is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with Code Dream.  If not, see <https://www.gnu.org/licenses/>.

#ifndef CODE_DREAM_OPTIONS_H
#define CODE_DREAM_OPTIONS_H

#include <stdbool.h>

typedef struct code_dream_options_t code_dream_options_t;

struct code_dream_options_t
{
  int screen_width;
  int screen_height;
  int screen_x;
  int screen_y;
  bool help;
  bool version;
  bool fullscreen;
  bool light;
  char *output;
  char *filename;
  char *theme;
  int n_seconds;
  int window;
};

#endif
