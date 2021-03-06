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

#ifndef CODE_IMAGE_SET_H
#define CODE_IMAGE_SET_H

#include <stdbool.h>

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>

#include "code-source.h"
#include "code-dream-image.h"
#include "code-dream-theme.h"

typedef struct code_image_set_t code_image_set_t;

struct code_image_set_t
{
  code_source_t *code_source;
  code_dream_theme_t *theme;
  char *font_path;
  int n_renderers;
  SDL_Renderer **renderers;
  code_dream_image_t ***images;
  long *n_images;
  TTF_Font *font;
  TTF_Font *cjk_font;
  int font_width;
  int font_height;
  bool loaded;
};

code_image_set_t *
code_image_set_create(const char *basedir,
                      code_source_t *code_source,
                      code_dream_theme_t *theme,
                      SDL_Renderer *renderer,
                      ...);

bool
code_image_set_loading(code_image_set_t *code_image_set);

int
code_image_set_load(void *data);

code_dream_image_t *
code_image_set_get_char_image_for_renderer(code_image_set_t *set,
                                           char *c,
                                           code_dream_face_t face,
                                           SDL_Renderer *renderer);

void
code_image_set_destroy(code_image_set_t *set);

#endif
