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

#include "code-dream-code-display.h"

code_dream_code_display_t *
code_dream_code_display_create(code_dream_char_info_set_t *char_info_set,
                               code_image_set_t *code_image_set,
                               int screen_width,
                               int screen_height)
{
  code_dream_code_display_t *display =
    (code_dream_code_display_t *)malloc(sizeof(code_dream_code_display_t));
  display->char_info_set = char_info_set;
  display->code_image_set = code_image_set;
  display->line_to_zoom_under = rand() % char_info_set->n_lines;
  display->total_time = 300;
  display->min_dist = code_image_set->font_width / 1000.0;
  display->max_dist = code_image_set->font_width / 5.0;
  display->speed = (display->max_dist - display->min_dist) / (double)display->total_time;
  display->dist = display->max_dist;
  display->screen_width = screen_width;
  display->screen_height = screen_height;
  return display;
}

void
code_dream_code_display_update(code_dream_code_display_t *display)
{
  display->dist -= display->speed;
}

void
code_dream_code_display_draw_char(code_dream_code_display_t *display,
                                  code_dream_char_info_t *char_info,
                                  code_image_set_t *code_image_set,
                                  SDL_Renderer *renderer)
{
  code_dream_image_t *image =
    code_image_set_get_char_image(display->code_image_set,
                                  char_info->c,
                                  char_info->type);
  int x = char_info->col * code_image_set->font_width;
  int y = (char_info->row - 1) * code_image_set->font_height;
  double code_width = code_image_set->font_width * 80;
  int zoom_y = code_image_set->font_height * display->line_to_zoom_under;
  SDL_FRect rect;
  rect.x = display->screen_width / 2 + (-code_width / 2 + x) / display->dist
    * display->screen_height / 400.0;
  rect.y = display->screen_height / 2 + (-zoom_y + y) / display->dist
    * display->screen_height / 400.0;
  rect.x += (rand() % 3 - 1) * display->screen_height / 200.0 / display->dist;
  rect.y += (rand() % 3 - 1) * display->screen_height / 200.0 / display->dist;
  rect.w = image->w / display->dist * display->screen_height / 400.0;
  rect.h = image->h / display->dist * display->screen_height / 400.0;
  double dist_range = display->max_dist - display->min_dist;
  int alpha = (dist_range - (display->dist - display->min_dist)) / dist_range
    * 256;
  if (alpha < 0) alpha = 0;
  if (alpha > 255) alpha = 255;
  SDL_SetTextureAlphaMod(image->image, alpha);
  SDL_RenderCopyF(renderer, image->image, NULL, &rect);
}

void
code_dream_code_display_draw(code_dream_code_display_t *display,
                             SDL_Renderer *renderer)
{
  int i = 0;
  for (i = 0; i < display->char_info_set->n_infos; ++i)
    {
      code_dream_char_info_t *char_info = display->char_info_set->infos[i];
      code_dream_code_display_draw_char(display,
                                        char_info,
                                        display->code_image_set,
                                        renderer);
    }
}

void
code_dream_code_display_destroy(code_dream_code_display_t *display)
{
  free(display);
}
