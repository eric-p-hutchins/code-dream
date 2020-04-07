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

#include "code-dream-char-info-set.h"

code_dream_char_info_set_t *
code_dream_char_info_set_create()
{
  code_dream_char_info_set_t *set =
    (code_dream_char_info_set_t*)malloc(sizeof(code_dream_char_info_set_t));
  set->n_infos = 0;
  set->infos = NULL;
  return set;
}

void
code_dream_char_info_set_add_char(code_dream_char_info_set_t *set,
                                  code_dream_char_info_t *char_info)
{
  ++set->n_infos;
  set->infos =
    (code_dream_char_info_t**)realloc(set->infos,
                                      sizeof(code_dream_char_info_t)
                                      * set->n_infos);
  set->infos[set->n_infos - 1] = char_info;
}

void
code_dream_char_info_set_destroy(code_dream_char_info_set_t *set)
{
  int i;
  for (i = 0; i < set->n_infos; ++i)
    {
      code_dream_char_info_destroy(set->infos[i]);
    }
  free(set);
}
