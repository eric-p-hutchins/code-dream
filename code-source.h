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

#ifndef CODE_SOURCE_H
#define CODE_SOURCE_H

#include "code-dream-char-info-set.h"

typedef struct code_source_t code_source_t;

struct code_source_t {
  char *filename;
};

code_source_t *code_source_create(const char *filename);
code_dream_char_info_set_t *code_source_get_char_info_set(code_source_t *code_source);

#endif
