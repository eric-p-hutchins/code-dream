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

#ifndef CODE_DREAM_SMOKE_LIST_H
#define CODE_DREAM_SMOKE_LIST_H

#include "code-dream-smoke.h"

typedef struct code_dream_smoke_list_t code_dream_smoke_list_t;

struct code_dream_smoke_list_t {
  SDL_Color color;
  int screen_width;
  int screen_height;
  int n_smokes;
  code_dream_smoke_t **smokes;
  Uint8 *pixels;
};

code_dream_smoke_list_t *code_dream_smoke_list_create(SDL_Color color,
                                                      int screen_width,
                                                      int screen_height);
void code_dream_smoke_list_update(code_dream_smoke_list_t *list);
void code_dream_smoke_list_draw_between(code_dream_smoke_list_t *list,
                                        SDL_Renderer *renderer,
                                        double min,
                                        double max);
void code_dream_smoke_list_destroy(code_dream_smoke_list_t *list);

#endif
