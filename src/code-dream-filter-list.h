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

#ifndef CODE_DREAM_FILTER_LIST_H
#define CODE_DREAM_FILTER_LIST_H

#include "code-dream-filter.h"

#include "SDL2/SDL_rect.h"

typedef struct code_dream_filter_list_t code_dream_filter_list_t;

struct code_dream_filter_list_t {
  int n_filters;
  code_dream_filter_t **filters;
};

code_dream_filter_list_t *code_dream_filter_list_create();
void code_dream_filter_list_add_filter(code_dream_filter_list_t *list,
                                       code_dream_filter_t *filter);
void code_dream_filter_list_apply(code_dream_filter_list_t *list,
                                  SDL_FRect *rect,
                                  uint8_t *state);
void code_dream_filter_list_destroy(code_dream_filter_list_t *list);

#endif
