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

#include "code-dream-smoke-list.h"

void
code_dream_smoke_list_add_cloud(code_dream_smoke_list_t *list, double z);

int n_clouds = 80;
int n_smokes_per_cloud = 8000;

code_dream_smoke_list_t *
code_dream_smoke_list_create(SDL_Color color,
                             int screen_width,
                             int screen_height)
{
  code_dream_smoke_list_t *list =
    (code_dream_smoke_list_t*)malloc(sizeof(code_dream_smoke_list_t));
  list->color = color;
  list->screen_width = screen_width;
  list->screen_height = screen_height;
  list->n_smokes = 0;
  list->smokes = NULL;
  int i;
  for (i = 1; i <= n_clouds; ++i)
    {
      code_dream_smoke_list_add_cloud(list, 64.0 / n_clouds * i);
    }
  list->pixels =
    (Uint8*)malloc(sizeof(Uint8) * screen_width * screen_height * 4);
  return list;
}

void
code_dream_smoke_list_add_smoke(code_dream_smoke_list_t *list,
                                double x,
                                double y,
                                double z)
{
  code_dream_smoke_t *smoke = code_dream_smoke_create(x, y, z, 0.2);
  ++list->n_smokes;
  list->smokes =
    (code_dream_smoke_t**)realloc(list->smokes,
                                  sizeof(code_dream_smoke_t*) * list->n_smokes);
  list->smokes[list->n_smokes - 1] = smoke;
}

void
code_dream_smoke_list_add_cloud(code_dream_smoke_list_t *list, double base_z)
{
  int field_x_range = 40;
  int field_y_range = 10;
  double base_x = rand() % field_x_range - field_x_range / 2;
  double base_y = rand() % field_y_range - field_y_range / 2;
  double cloud_x_range = 8.0;
  int cloud_x_range_int = cloud_x_range * 100;
  double cloud_y_range = 3.0;
  int cloud_y_range_int = cloud_y_range * 100;
  double cloud_z_range = 8.0;
  int cloud_z_range_int = cloud_z_range * 100;
  int i;
  for (i = 0; i < n_smokes_per_cloud; ++i)
    {
      double x = base_x
        + ((rand() % cloud_x_range_int / 4
            + rand() % cloud_x_range_int / 4
            + rand() % cloud_x_range_int / 4
            + rand() % cloud_x_range_int / 4)
           / 100.0);
      double y = base_y
        + ((rand() % cloud_y_range_int / 4
            + rand() % cloud_y_range_int / 4
            + rand() % cloud_y_range_int / 4
            + rand() % cloud_y_range_int / 4)
           / 100.0);
      double z = base_z
        + ((rand() % cloud_z_range_int / 4
            + rand() % cloud_z_range_int / 4
            + rand() % cloud_z_range_int / 4
            + rand() % cloud_z_range_int / 4)
           / 100.0);
      code_dream_smoke_list_add_smoke(list, x, y, z);
    }
}

void
code_dream_smoke_list_draw_between(code_dream_smoke_list_t *list,
                                   SDL_Renderer *renderer,
                                   double min,
                                   double max)
{
  SDL_Texture *tex = SDL_CreateTexture(renderer,
                                       SDL_PIXELFORMAT_RGBA32,
                                       SDL_TEXTUREACCESS_TARGET,
                                       list->screen_width,
                                       list->screen_height);
  memset(list->pixels, 0, list->screen_width * 4 * list->screen_height);
  int i, j, k;
  for (i = 0; i < list->n_smokes; ++i)
    {
      code_dream_smoke_t *smoke = list->smokes[i];
      if (smoke->z <= max && smoke->z > min)
        {
          code_dream_smoke_draw(smoke,
                                list->pixels,
                                list->color.r,
                                list->color.g,
                                list->color.b,
                                list->screen_width,
                                list->screen_height);
        }
    }
  SDL_UpdateTexture(tex, NULL, list->pixels, list->screen_width * 4);
  SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
  SDL_RenderCopy(renderer, tex, NULL, NULL);
  SDL_DestroyTexture(tex);
}

void
code_dream_smoke_list_update(code_dream_smoke_list_t *list)
{
  int i;
  for (i = 0; i < list->n_smokes; ++i)
    {
      list->smokes[i]->z -= 0.025;
      if (list->smokes[i]->z < 0.01)
        {
          code_dream_smoke_destroy(list->smokes[i]);
          list->smokes[i] = NULL;
        }
    }
  for (i = 0; i < list->n_smokes; ++i)
    {
      if (list->smokes[i] == NULL)
        {
          int j;
          for (j = i; j < list->n_smokes - 1; ++j)
            {
              list->smokes[j] = list->smokes[j + 1];
            }
          --list->n_smokes;
          --i;
        }
    }
  if (list->n_smokes < n_clouds * n_smokes_per_cloud)
    {
      code_dream_smoke_list_add_cloud(list, 64.0);
    }
}

void
code_dream_smoke_list_destroy(code_dream_smoke_list_t *list)
{
  int i;
  for (i = 0; i < list->n_smokes; ++i)
    {
      code_dream_smoke_destroy(list->smokes[i]);
    }
  free(list);
}
