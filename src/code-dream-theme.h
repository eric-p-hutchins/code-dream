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

#ifndef CODE_DREAM_THEME_H
#define CODE_DREAM_THEME_H

#include <stdbool.h>

#include "SDL2/SDL_pixels.h"

typedef struct code_dream_theme_t code_dream_theme_t;

struct code_dream_theme_t
{
  SDL_Color background_color;
  SDL_Color default_color;
  SDL_Color preproc_color;
  SDL_Color string_color;
  SDL_Color var_color;
  SDL_Color type_color;
  SDL_Color func_color;
  SDL_Color keyword_color;
  SDL_Color keyvalue_color;
  SDL_Color comment_color;
};

code_dream_theme_t *
code_dream_theme_create(const char *name,
                        bool light,
                        const char *basedir);

void code_dream_theme_destroy(code_dream_theme_t *theme);

#endif
