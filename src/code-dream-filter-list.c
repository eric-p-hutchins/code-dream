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

#include <stdlib.h>

#include "code-dream-filter-list.h"

code_dream_filter_list_t *
code_dream_filter_list_create()
{
  code_dream_filter_list_t *list =
    (code_dream_filter_list_t *)malloc(sizeof(code_dream_filter_list_t));
  list->n_filters = 0;
  list->filters = NULL;
  return list;
}

void
code_dream_filter_list_add_filter(code_dream_filter_list_t *list,
                                  code_dream_filter_t *filter)
{
  ++list->n_filters;
  list->filters =
    (code_dream_filter_t **)realloc(list->filters,
                                        sizeof(code_dream_filter_t*)
                                        * list->n_filters);
  list->filters[list->n_filters - 1] = filter;
}

void code_dream_filter_list_apply(code_dream_filter_list_t *list,
                                  SDL_FRect *rect,
                                  uint8_t *state)
{
  int i;
  for (i = 0; i < list->n_filters; ++i)
    {
      code_dream_filter_apply(list->filters[i],
                              rect,
                              state + i);
    }
}

void
code_dream_filter_list_destroy(code_dream_filter_list_t *list)
{
  int i;
  for (i = 0; i < list->n_filters; ++i)
    {
      code_dream_filter_destroy(list->filters[i]);
    }
  free(list);
}
