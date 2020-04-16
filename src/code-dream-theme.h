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

#ifndef CODE_DREAM_THEME_H
#define CODE_DREAM_THEME_H

#include <stdbool.h>

#include "code-dream-format-type.h"
#include "code-dream-face.h"

typedef struct code_dream_theme_t code_dream_theme_t;

struct code_dream_theme_t
{
  SDL_Color background_color;
  code_dream_face_t default_face;
  code_dream_face_t preproc_face;
  code_dream_face_t string_face;
  code_dream_face_t var_face;
  code_dream_face_t type_face;
  code_dream_face_t func_face;
  code_dream_face_t keyword_face;
  code_dream_face_t keyvalue_face;
  code_dream_face_t comment_face;
};

code_dream_theme_t *
code_dream_theme_create(const char *name,
                        bool light,
                        const char *basedir);

code_dream_face_t
code_dream_theme_format_type_to_face(code_dream_theme_t *theme,
                                     code_dream_format_type_t type);

void code_dream_theme_destroy(code_dream_theme_t *theme);

#endif
