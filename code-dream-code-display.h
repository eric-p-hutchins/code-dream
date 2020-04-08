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

#ifndef CODE_DREAM_CODE_DISPLAY_H
#define CODE_DREAM_CODE_DISPLAY_H

#include "SDL2/SDL_video.h"

#include "code-dream-char-info-set.h"
#include "code-image-set.h"

typedef struct code_dream_code_display_t code_dream_code_display_t;

struct code_dream_code_display_t
{
  code_dream_char_info_set_t *char_info_set;
  code_image_set_t *code_image_set;
  int line_to_zoom_under;
  int total_time;
  double min_dist;
  double max_dist;
  double speed;
  double dist;
};

code_dream_code_display_t *
code_dream_code_display_create(code_dream_char_info_set_t *char_info_set,
                               code_image_set_t *code_image_set);

void
code_dream_code_display_update(code_dream_code_display_t *display);

void
code_dream_code_display_draw(code_dream_code_display_t *display,
                             SDL_Window *window);

void
code_dream_code_display_destroy(code_dream_code_display_t *display);

#endif
