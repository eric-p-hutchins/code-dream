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

#ifndef CODE_DREAM_SMOKE_H
#define CODE_DREAM_SMOKE_H

#include "SDL2/SDL_render.h"

typedef struct code_dream_smoke_t code_dream_smoke_t;

struct code_dream_smoke_t {
  double x;
  double y;
  double z;
  double size;
};

code_dream_smoke_t *code_dream_smoke_create(double x,
                                            double y,
                                            double z,
                                            double size);
void code_dream_smoke_draw(code_dream_smoke_t *smoke,
                           Uint8 *pixels,
                           int r,
                           int g,
                           int b,
                           int screen_width,
                           int screen_height);
void code_dream_smoke_destroy(code_dream_smoke_t *smoke);

#endif
