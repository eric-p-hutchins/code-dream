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

#ifndef CODE_DREAM_GIF_WRITER_H
#define CODE_DREAM_GIF_WRITER_H

#include "SDL2/SDL_render.h"
#include "SDL2/SDL_surface.h"

#include "gif_lib.h"

#include "code-image-set.h"
#include "code-dream-code-display-set.h"

typedef struct code_dream_gif_writer_t code_dream_gif_writer_t;

struct code_dream_gif_writer_t
{
  void *priv;
};

typedef struct code_dream_gif_writer_priv_t code_dream_gif_writer_priv_t;

struct code_dream_gif_writer_priv_t
{
  SDL_Renderer *renderer;
  SDL_Surface *surface;
  code_image_set_t *code_image_set;
  GifFileType *gif_file;
};

code_dream_gif_writer_t *
code_dream_gif_writer_create(code_source_t *code_source,
                             int screen_width,
                             int screen_height);

SDL_Renderer *
code_dream_gif_writer_get_renderer(code_dream_gif_writer_t *writer);

SDL_Surface *
code_dream_gif_writer_get_surface(code_dream_gif_writer_t *writer);

code_image_set_t *
code_dream_gif_writer_get_code_image_set(code_dream_gif_writer_t *writer);

void
code_dream_gif_writer_draw_frame(code_dream_gif_writer_t *writer,
                                 code_dream_code_display_set_t *displays);

void code_dream_gif_writer_write_gif(code_dream_gif_writer_t *writer);

void code_dream_gif_writer_destroy(code_dream_gif_writer_t *writer);

#endif
