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

#ifndef CODE_DREAM_LOADING_SCREEN_H
#define CODE_DREAM_LOADING_SCREEN_H

#include "SDL_render.h"

typedef struct code_dream_loading_screen_t code_dream_loading_screen_t;

struct code_dream_loading_screen_t
{
  SDL_Renderer *renderer;
  int screen_width;
  int screen_height;
  int t;
};

code_dream_loading_screen_t *
code_dream_loading_screen_create(SDL_Renderer *renderer,
                                 int screen_width,
                                 int screen_height);

void
code_dream_loading_screen_update(code_dream_loading_screen_t *loading_screen);

void
code_dream_loading_screen_draw(code_dream_loading_screen_t *loading_screen);

void
code_dream_loading_screen_destroy(code_dream_loading_screen_t *loading_screen);

#endif
