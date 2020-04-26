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
  code_image_set->font = TTF_OpenFont(code_image_set->font_path, 128);
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
  code_image_set->n_images = (int*)malloc(sizeof(int)
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
code_image_set_create_images_for_renderer(code_image_set_t *code_image_set,
                                          code_dream_char_info_t *char_info,
                                          int renderer_index)
{
  SDL_Renderer *renderer = code_image_set->renderers[renderer_index];
  code_dream_theme_t *theme = code_image_set->theme;
  code_dream_face_t face =
    code_dream_theme_format_type_to_face(theme, char_info->type);
  char c[2];
  c[0] = char_info->c;
  c[1] = '\0';

  if (face.weight == CD_BOLD)
    {
      TTF_SetFontStyle(code_image_set->font, TTF_STYLE_BOLD);
    }
  else
    {
      TTF_SetFontStyle(code_image_set->font, TTF_STYLE_NORMAL);
    }
  
  SDL_Surface *char_surface;
  int gradient_resolution = 8;
  int i;
  for (i = 0; i <= (gradient_resolution - 1); ++i)
    {
      SDL_Color color = face.color;
      double alpha = i / (double)(gradient_resolution - 1);
      color.r =
        color.r * alpha + theme->background_color.r * (1.0 - alpha);
      if (color.r < 0) color.r = 0;
      if (color.r > 255) color.r = 255;
      color.g =
        color.g * alpha + theme->background_color.g * (1.0 - alpha);
      if (color.g < 0) color.g = 0;
      if (color.g > 255) color.g = 255;
      color.b =
        color.b * alpha + theme->background_color.b * (1.0 - alpha);
      if (color.b < 0) color.b = 0;
      if (color.b > 255) color.b = 255;
      code_dream_face_t alpha_face = (code_dream_face_t){color, face.weight};
      if (code_image_set_get_char_image_for_renderer(code_image_set,
                                                     char_info->c,
                                                     alpha_face,
                                                     renderer) != NULL)
        {
          continue;
        }
      char_surface = TTF_RenderText_Blended(code_image_set->font,
                                            c,
                                            color);
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
        code_dream_image_create(char_info->c,
                                alpha_face,
                                texture,
                                code_image_set->font_width * char_info->col,
                                code_image_set->font_height * (char_info->row - 1),
                                char_surface->w,
                                char_surface->h);
      int n_images = ++code_image_set->n_images[renderer_index];
      code_image_set->images[renderer_index] =
        (code_dream_image_t **)realloc(code_image_set->images[renderer_index],
                                       sizeof(code_dream_image_t*)
                                       * n_images);
      code_image_set->images[renderer_index][n_images - 1] =
        image;
    }
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
  int i;
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
  return NULL;
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

  int i, j, k;
  for (j = 0; j < n_sets; ++j)
    {
      code_dream_char_info_set_t *set = sets[j];
      for(i = 0; i < set->n_infos; ++i)
        {
          code_dream_face_t face =
            code_dream_theme_format_type_to_face(code_image_set->theme,
                                                 set->infos[i]->type);
          for (k = 0; k < code_image_set->n_renderers; ++k)
            {
              SDL_Renderer *renderer =
                code_image_set->renderers[k];
              code_dream_image_t *image =
                code_image_set_get_char_image_for_renderer(code_image_set,
                                                           set->infos[i]->c,
                                                           face,
                                                           renderer);
              // If the image with character and type doesn't exist
              // yet for this renderer, then create it
              if (image == NULL)
                {
                  code_image_set_create_images_for_renderer(code_image_set,
                                                            set->infos[i],
                                                            k);
                }
            }
        }
    }
  TTF_CloseFont(code_image_set->font);
  code_image_set->loaded = true;
  return 0;
}

void
code_image_set_destroy(code_image_set_t *code_image_set)
{
  int i, j;
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
