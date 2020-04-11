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

SDL_Color CODR_DEFAULT_COLOR = {238,238,236};
SDL_Color CODR_PREPROC_COLOR = {223,144,214};
SDL_Color CODR_STRING_COLOR = {233,185,110};
SDL_Color CODR_VAR_COLOR = {252,175,61};
SDL_Color CODR_TYPE_COLOR = {141,196,255};
SDL_Color CODR_FUNC_COLOR = {252,233,79};
SDL_Color CODR_KEYWORD_COLOR = {180,249,112};
SDL_Color CODR_KEYVALUE_COLOR = {232,178,227};
SDL_Color CODR_COMMENT_COLOR = {116,210,24};

code_image_set_t *
code_image_set_create(const char *basedir,
                      code_source_t *code_source,
                      SDL_Renderer *renderer)
{
  code_image_set_t *code_image_set =
    (code_image_set_t*)malloc(sizeof(code_image_set_t));
  code_image_set->code_source = code_source;
  code_image_set->renderer = renderer;
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
  if (TTF_SizeUTF8(code_image_set->font,
                   "A",
                   &(code_image_set->font_width),
                   &(code_image_set->font_height))) {
    
      fprintf (stderr, "Couldn't get font size: %s\n", TTF_GetError());
      TTF_CloseFont(code_image_set->font);
      free(code_image_set);
      return NULL;
  }
  code_image_set->images = NULL;
  code_image_set->n_images = 0;
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

code_dream_image_t *
code_image_set_create_image(code_image_set_t *code_image_set,
                            code_dream_char_info_t *char_info)
{
  SDL_Color color = CODR_DEFAULT_COLOR;
  switch(char_info->type)
    {
    case CODR_PREPROC:
      color = CODR_PREPROC_COLOR;
      break;
    case CODR_STRING:
      color = CODR_STRING_COLOR;
      break;
    case CODR_KEYWORD:
      color = CODR_KEYWORD_COLOR;
      break;
    case CODR_TYPE:
      color = CODR_TYPE_COLOR;
      break;
    case CODR_FUNCTION:
      color = CODR_FUNC_COLOR;
      break;
    case CODR_IDENTIFIER:
      color = CODR_VAR_COLOR;
      break;
    case CODR_KEYVALUE:
      color = CODR_KEYVALUE_COLOR;
      break;
    case CODR_COMMENT:
      color = CODR_COMMENT_COLOR;
      break;
    default:
      color = CODR_DEFAULT_COLOR;
      break;
    }
  char c[2];
  c[0] = char_info->c;
  c[1] = '\0';
  SDL_Surface *char_surface = TTF_RenderText_Blended(code_image_set->font,
                                                     c,
                                                     color);
  if (char_surface == NULL)
    {
      fprintf(stderr, "error: %s\n", TTF_GetError());
      return NULL;
    }
  SDL_Texture *texture =
    SDL_CreateTextureFromSurface(code_image_set->renderer, char_surface);
  if (texture == NULL)
    {
      fprintf(stderr, "Error creating texture: %s\n", SDL_GetError());
    }
  code_dream_image_t *image =
    code_dream_image_create(char_info->c,
                            char_info->type,
                            texture,
                            code_image_set->font_width * char_info->col,
                            code_image_set->font_height * (char_info->row - 1),
                            char_surface->w,
                            char_surface->h);
  return image;
}

code_dream_image_t *
code_image_set_get_char_image(code_image_set_t *set,
                              char c,
                              code_dream_format_type_t type)
{
  int i;
  for (i = 0; i < set->n_images; ++i)
    {
      if (set->images[i]->c == c && set->images[i]->type == type)
        {
          return set->images[i];
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

  int i, j;
  for (j = 0; j < n_sets; ++j)
    {
      code_dream_char_info_set_t *set = sets[j];
      for(i = 0; i < set->n_infos; ++i)
        {
          code_dream_image_t *image =
            code_image_set_get_char_image(code_image_set,
                                          set->infos[i]->c,
                                          set->infos[i]->type);
          // If the image with character and type doesn't exist yet,
          // then create it
          if (image == NULL)
            {
              image = code_image_set_create_image(code_image_set,
                                                  set->infos[i]);
              ++code_image_set->n_images;
              code_image_set->images =
                (code_dream_image_t **)realloc(code_image_set->images,
                                               sizeof(code_dream_image_t*)
                                               * code_image_set->n_images);
              code_image_set->images[code_image_set->n_images - 1] = image;
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
  int i;
  for (i = 0; i < code_image_set->n_images; ++i)
    {
      code_dream_image_destroy(code_image_set->images[i]);
    }
  free(code_image_set->images);
  free(code_image_set->font_path);
  free(code_image_set);
}
