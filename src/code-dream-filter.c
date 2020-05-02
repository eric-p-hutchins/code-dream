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

#include "code-dream-filter.h"

code_dream_filter_t *
code_dream_filter_create(uint8_t (*func)(SDL_FRect *, uint8_t))
{
  code_dream_filter_t *filter =
    (code_dream_filter_t*)malloc(sizeof(code_dream_filter_t));
  filter->func = func;
  return filter;
}

void
code_dream_filter_apply(code_dream_filter_t *filter,
                        SDL_FRect *rect,
                        uint8_t *state)
{
  *state = filter->func(rect, *state);
}

void
code_dream_filter_destroy(code_dream_filter_t *filter)
{
  free(filter);
}
