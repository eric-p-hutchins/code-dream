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

#ifndef CODE_DREAM_CHAR_INFO_SET_H
#define CODE_DREAM_CHAR_INFO_SET_H

#include <stdlib.h>

#include "code-dream-char-info.h"

typedef struct code_dream_char_info_set_t code_dream_char_info_set_t;

struct code_dream_char_info_set_t
{
  int n_infos;
  code_dream_char_info_t **infos;
  int n_lines;
};

code_dream_char_info_set_t *code_dream_char_info_set_create();
void code_dream_char_info_set_add_char(code_dream_char_info_set_t *set,
                                       code_dream_char_info_t *char_info);
void code_dream_char_info_set_destroy(code_dream_char_info_set_t *set);

#endif
