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

#ifndef CODE_DREAM_CHAR_INFO_H
#define CODE_DREAM_CHAR_INFO_H

#include <stdlib.h>

#include "code-dream-format-type.h"

typedef struct code_dream_char_info_t code_dream_char_info_t;

struct code_dream_char_info_t
{
  char *c;
  code_dream_format_type_t type;
  int row;
  int col;
};

code_dream_char_info_t *code_dream_char_info_create(const char *c,
                                                    code_dream_format_type_t type,
                                                    int row,
                                                    int col);
void code_dream_char_info_destroy(code_dream_char_info_t *char_info);

#endif
