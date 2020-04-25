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
  int x = char_info->col * code_image_set->font_width;
  int y = (char_info->row - 1) * code_image_set->font_height;
  int zoom_y = code_image_set->font_height * display->line_to_zoom_under;
  double code_width = code_image_set->font_width * 80;
  SDL_FRect rect;
  rect.x = display->screen_width / 2 + (-code_width / 2 + x) / display->dist
    * display->screen_height / 400.0;
  rect.y = display->screen_height / 2 + (-zoom_y + y) / display->dist
    * display->screen_height / 400.0;
  rect.x += (rand() % 3 - 1) * display->screen_height / 200.0 / display->dist;
  rect.y += (rand() % 3 - 1) * display->screen_height / 200.0 / display->dist;
  rect.w = code_image_set->font_width / display->dist * display->screen_height / 400.0;
  rect.h = code_image_set->font_height / display->dist * display->screen_height / 400.0;
  if (rect.x >= display->screen_width
      || rect.y >= display->screen_height
      || (rect.x + rect.w) <= 0
      || (rect.y + rect.h) <= 0)
    {
      return;
    }

  code_dream_theme_t *theme = code_image_set->theme;
  code_dream_face_t face =
    code_dream_theme_format_type_to_face(theme, char_info->type);
  SDL_Color color = face.color;
  double dist_range = display->max_dist - display->min_dist;
  double how_close = dist_range - (display->dist - display->min_dist);
  double alpha = 0;
  int alpha_level = how_close * 16 / dist_range;
  if (alpha_level < 0) alpha_level = 0;
  if (alpha_level > 15) alpha_level = 15;
  alpha = alpha_level / 15.0;
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
  char c[2];
  c[0] = char_info->c;
  c[1] = '\0';
  code_dream_image_t *image =
    code_image_set_get_char_image_for_renderer(display->code_image_set,
                                               char_info->c,
                                               (code_dream_face_t){color,
                                                                   face.weight},
                                               renderer);

  if (image != NULL)
    {
      SDL_RenderCopyF(renderer, image->image, NULL, &rect);
    }
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
