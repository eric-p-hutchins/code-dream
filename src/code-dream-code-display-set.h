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

#ifndef CODE_DREAM_CODE_DISPLAY_SET_H
#define CODE_DREAM_CODE_DISPLAY_SET_H

#include "SDL2/SDL_render.h"

#include "code-dream-code-display.h"
#include "code-dream-filter-list.h"
#include "code-dream-smoke-list.h"
#include "code-image-set.h"
#include "code-source.h"

typedef struct code_dream_code_display_set_t code_dream_code_display_set_t;

struct code_dream_code_display_set_t
{
  code_source_t *code_source;
  code_image_set_t *code_image_set;
  int n_displays;
  code_dream_code_display_t **displays;
  code_dream_filter_list_t *filter_list;
  int screen_width;
  int screen_height;
  code_dream_smoke_list_t *smoke_list;
};

code_dream_code_display_set_t *
code_dream_code_display_set_create(code_source_t *code_source,
                                   code_image_set_t *code_image_set,
                                   int screen_width,
                                   int screen_height,
                                   code_dream_filter_list_t *filter_list);

void
code_dream_code_display_set_draw(code_dream_code_display_set_t *set,
                                 SDL_Renderer *renderer);

void
code_dream_code_display_set_update(code_dream_code_display_set_t *set);

void
code_dream_code_display_set_add_display(code_dream_code_display_set_t *set);

void
code_dream_code_display_set_destroy(code_dream_code_display_set_t *set);

#endif
