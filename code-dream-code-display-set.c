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

#include "code-dream-code-display-set.h"

code_dream_code_display_set_t *
code_dream_code_display_set_create(code_source_t *code_source,
                                   code_image_set_t *code_image_set)
{
  code_dream_code_display_set_t *set =
    (code_dream_code_display_set_t *)malloc(sizeof(code_dream_code_display_set_t));
  set->n_displays = 0;
  set->displays = NULL;
  set->code_source = code_source;
  set->code_image_set = code_image_set;
  return set;
}

void
code_dream_code_display_set_draw(code_dream_code_display_set_t *set,
                                 SDL_Window *window)
{
  int i;
  for (i = 0; i < set->n_displays; ++i)
    {
      code_dream_code_display_draw(set->displays[i],
                                   window);
    }
}

void
code_dream_code_display_set_update(code_dream_code_display_set_t *set)
{
  int i;
  for (i = 0; i < set->n_displays; ++i)
    {
      code_dream_code_display_update(set->displays[i]);
      if (set->displays[i]->dist < set->displays[i]->min_dist)
        {
          code_dream_code_display_destroy(set->displays[i]);
          set->displays[i] = NULL;
        }
    }
  for (i = 0; i < set->n_displays; ++i)
    {
      if (set->displays[i] == NULL)
        {
          int j;
          for (j = i; j < set->n_displays - 1; ++j)
            {
              set->displays[j] = set->displays[j + 1];
            }
          --set->n_displays;
          --i;
        }
    }
  if (set->n_displays < 3)
    {
      code_dream_code_display_set_add_display(set);
    }
}

void
code_dream_code_display_set_add_display(code_dream_code_display_set_t *set)
{
  code_dream_char_info_set_t **sets;
  size_t n_sets;
  code_source_get_char_info_sets(set->code_source, &sets, &n_sets);

  code_dream_char_info_set_t *char_info_set = sets[rand() % n_sets];
  code_dream_code_display_t *display =
    code_dream_code_display_create(char_info_set, set->code_image_set);
  ++set->n_displays;
  set->displays =
    (code_dream_code_display_t**)realloc(set->displays,
                                         sizeof(code_dream_code_display_t*)
                                         * set->n_displays);
  set->displays[set->n_displays - 1] = display;
}
