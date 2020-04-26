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

#include "code-image-set.h"

code_image_set_t *
code_image_set_create(const char *basedir,
                      code_source_t *code_source,
                      code_dream_theme_t *theme,
                      SDL_Renderer *renderer,
                      ...)
{
  code_image_set_t *code_image_set =
    (code_image_set_t*)malloc(sizeof(code_image_set_t));
  code_image_set->code_source = code_source;
  code_image_set->theme = theme;

  code_image_set->n_renderers = 1;
  code_image_set->renderers =
    (SDL_Renderer**)malloc(sizeof(SDL_Renderer*));
  code_image_set->renderers[0] = renderer;
  va_list more_renderers;

  va_start(more_renderers, renderer);
  SDL_Renderer *another_renderer = va_arg(more_renderers, SDL_Renderer *);
  while (another_renderer != NULL)
    {
      size_t n = ++code_image_set->n_renderers;
      code_image_set->renderers =
        (SDL_Renderer**)realloc(code_image_set->renderers,
                                sizeof(SDL_Renderer*) * n);
      code_image_set->renderers[n - 1] = another_renderer;
      another_renderer = va_arg(more_renderers, SDL_Renderer *);
    }
  va_end(more_renderers);

  code_image_set->font_path = (char*)malloc(strlen(basedir)
                                              + strlen("/DejaVuSansMono.ttf")
                                              + 1);
  strcpy(code_image_set->font_path, basedir);
  strcat(code_image_set->font_path, "/DejaVuSansMono.ttf");
  FILE *file = fopen(code_image_set->font_path, "r");
  if (file == NULL)
    {
      free(code_image_set->font_path);
      code_image_set->font_path = (char*)malloc(strlen(basedir)
                                                + strlen("/../DejaVuSansMono.ttf")
                                                + 1);
      strcpy(code_image_set->font_path, basedir);
      strcat(code_image_set->font_path, "/../DejaVuSansMono.ttf");
      file = fopen(code_image_set->font_path, "r");
    }
  if (file == NULL)
    {
      free(code_image_set->font_path);
      code_image_set->font_path = (char*)malloc(strlen(DATADIR)
                                                + strlen("/DejaVuSansMono.ttf")
                                                + 1);
      strcpy(code_image_set->font_path, DATADIR);
      strcat(code_image_set->font_path, "/DejaVuSansMono.ttf");
      file = fopen(code_image_set->font_path, "r");
    }
  if (file == NULL)
    {
      fprintf(stderr, "%s\n", "Error: can't find DejaVuSansMono.ttf");
      code_image_set_destroy(code_image_set);
      return NULL;
    }
  fclose(file);
  code_image_set->font = TTF_OpenFont(code_image_set->font_path, 64);
  if (code_image_set->font == NULL)
    {
      fprintf (stderr, "Couldn't create font: %s\n", TTF_GetError());
      free(code_image_set);
      return NULL;
    }
  TTF_SetFontHinting(code_image_set->font, TTF_HINTING_NONE);
  if (TTF_SizeUTF8(code_image_set->font,
                   "A",
                   &(code_image_set->font_width),
                   &(code_image_set->font_height))) {
    
      fprintf (stderr, "Couldn't get font size: %s\n", TTF_GetError());
      TTF_CloseFont(code_image_set->font);
      free(code_image_set);
      return NULL;
  }
  code_image_set->images =
    (code_dream_image_t***)malloc(sizeof(code_dream_image_t**)
                                  * code_image_set->n_renderers);
  code_image_set->n_images = (long*)malloc(sizeof(long)
                                          * code_image_set->n_renderers);
  int i;
  for (i = 0; i < code_image_set->n_renderers; ++i)
    {
      code_image_set->images[i] = NULL;
      code_image_set->n_images[i] = 0;
    }
  code_image_set->loaded = false;

  SDL_Thread *thread =
    SDL_CreateThread(code_image_set_load,
                     "code_image_set_load",
                     code_image_set);
  SDL_DetachThread(thread);
  return code_image_set;
}

bool
code_image_set_loading(code_image_set_t *code_image_set)
{
  return !code_image_set->loaded;
}

void
code_image_set_create_image_for_renderer(code_image_set_t *code_image_set,
                                         char c,
                                         code_dream_face_t face,
                                         int row,
                                         int col,
                                         int renderer_index)
{
  char c_str[2];
  c_str[0] = c;
  c_str[1] = '\0';
  if (face.weight == CD_BOLD)
    {
      TTF_SetFontStyle(code_image_set->font, TTF_STYLE_BOLD);
    }
  else
    {
      TTF_SetFontStyle(code_image_set->font, TTF_STYLE_NORMAL);
    }
  SDL_Surface *char_surface = TTF_RenderText_Blended(code_image_set->font,
                                                     c_str,
                                                     face.color);
  if (char_surface == NULL)
    {
      fprintf(stderr, "error: %s\n", TTF_GetError());
      return;
    }
  SDL_Texture *texture =
    SDL_CreateTextureFromSurface(code_image_set->renderers[renderer_index],
                                 char_surface);
  if (texture == NULL)
    {
      fprintf(stderr, "Error creating texture: %s\n", SDL_GetError());
    }
  code_dream_image_t *image =
    code_dream_image_create(c,
                            face,
                            texture,
                            code_image_set->font_width * col,
                            code_image_set->font_height * (row - 1),
                            char_surface->w,
                            char_surface->h);
  SDL_FreeSurface(char_surface);
  long n_images = ++code_image_set->n_images[renderer_index];
  code_image_set->images[renderer_index] =
    (code_dream_image_t **)realloc(code_image_set->images[renderer_index],
                                   sizeof(code_dream_image_t*)
                                   * n_images);
  code_image_set->images[renderer_index][n_images - 1] =
    image;
}

int
code_image_set_get_renderer_index(code_image_set_t *set, SDL_Renderer *renderer)
{
  int i;
  for (i = 0; i < set->n_renderers; ++i)
    {
      if (renderer == set->renderers[i])
        {
          return i;
        }
    }
  return -1;
}

code_dream_image_t *
code_image_set_get_char_image_for_renderer(code_image_set_t *set,
                                           char c,
                                           code_dream_face_t face,
                                           SDL_Renderer *renderer)
{
  int renderer_index = code_image_set_get_renderer_index(set, renderer);
  if (renderer_index < 0)
    {
      fprintf(stderr, "Error: Renderer not found for code-image-set!\n");
      return NULL;
    }
  long i;
  for (i = 0; i < set->n_images[renderer_index]; ++i)
    {
      if (set->images[renderer_index][i]->c == c
          && set->images[renderer_index][i]->face.color.r == face.color.r
          && set->images[renderer_index][i]->face.color.g == face.color.g
          && set->images[renderer_index][i]->face.color.b == face.color.b
          && set->images[renderer_index][i]->face.weight == face.weight)
        {
          return set->images[renderer_index][i];
        }
    }
  long max_images = 2000;
  if (set->n_images[renderer_index] >= max_images)
    {
      long i;
      for (i = 0; i <= set->n_images[renderer_index] - max_images; ++i)
        {
          code_dream_image_destroy(set->images[renderer_index][i]);
        }
      for (i = 0; i < max_images - 1; ++i)
        {
          set->images[renderer_index][i] =
            set->images[renderer_index][i + set->n_images[renderer_index] - max_images + 1];
        }
      set->n_images[renderer_index] = max_images - 1;
      set->images[renderer_index] =
        (code_dream_image_t**)realloc(set->images[renderer_index],
                                      sizeof(code_dream_image_t*)
                                      * set->n_images[renderer_index]);
    }
  code_image_set_create_image_for_renderer(set,
                                           c,
                                           face,
                                           1, 0,
                                           renderer_index);
  return code_image_set_get_char_image_for_renderer(set,
                                                    c,
                                                    face,
                                                    renderer);
}

int
code_image_set_load(void *data)
{
  code_image_set_t *code_image_set = (code_image_set_t *)data;
  while (code_source_loading(code_image_set->code_source))
    {
      ;
    }

  code_dream_char_info_set_t **sets;
  size_t n_sets;
  code_source_get_char_info_sets(code_image_set->code_source, &sets, &n_sets);

  code_image_set->loaded = true;
  return 0;
}

void
code_image_set_destroy(code_image_set_t *code_image_set)
{
  TTF_CloseFont(code_image_set->font);
  int j;
  long i;
  for (j = 0; j < code_image_set->n_renderers; ++j)
    {
      for (i = 0; i < code_image_set->n_images[j]; ++i)
        {
          code_dream_image_destroy(code_image_set->images[j][i]);
        }
      free(code_image_set->images[j]);
    }
  free(code_image_set->n_images);
  free(code_image_set->images);
  free(code_image_set->font_path);
  free(code_image_set);
}
