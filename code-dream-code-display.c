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
code_dream_code_display_create(code_image_set_t *images)
{
  code_dream_code_display_t *display =
    (code_dream_code_display_t *)malloc(sizeof(code_dream_code_display_t));
  display->images = images;
  display->line_to_zoom_under = rand() % images->n_lines;
  display->total_time = 300;
  display->min_dist = images->font_width / 1000.0;
  display->max_dist = images->font_width / 5.0;
  display->speed = (display->max_dist - display->min_dist) / (double)display->total_time;
  display->dist = display->max_dist;
  return display;
}

void
code_dream_code_display_update(code_dream_code_display_t *display)
{
  display->dist -= display->speed;
  if (display->dist < display->min_dist)
    {
      display->line_to_zoom_under =
        rand() % display->images->n_lines;
      display->dist = display->max_dist;
    }
}

void
code_dream_code_display_draw_image(code_dream_code_display_t *display,
                                   code_image_set_t *code_image_set,
                                   code_dream_image_t *image,
                                   SDL_Window *window)
{
  int screen_width, screen_height;
  SDL_GetWindowSize(window, &screen_width, &screen_height);
  double code_width = code_image_set->font_width * 80;
  int zoom_y = code_image_set->font_height * display->line_to_zoom_under;
  SDL_Rect rect;
  rect.x = screen_width / 2 + (-code_width / 2 + image->x) / display->dist;
  rect.y = screen_height / 2 + (-zoom_y + image->y) / display->dist;
  rect.x += rand() % 3 + 1;
  rect.y += rand() % 3 + 1;
  rect.w = image->w / display->dist;
  rect.h = image->h / display->dist;
  double dist_range = display->max_dist - display->min_dist;
  int alpha = (dist_range - (display->dist - display->min_dist)) / dist_range
    * 256;
  if (alpha < 0) alpha = 0;
  if (alpha > 255) alpha = 255;
  SDL_SetTextureAlphaMod(image->image, alpha);
  SDL_Renderer *renderer = SDL_GetRenderer(window);
  SDL_RenderCopy(renderer, image->image, NULL, &rect);
}

void
code_dream_code_display_draw(code_dream_code_display_t *display,
                             SDL_Window *window)
{
  int i = 0;
  for (i = 0; i < display->images->n_images; ++i)
    {
      code_dream_image_t *image = display->images->images[i];
      code_dream_code_display_draw_image(display,
                                         display->images,
                                         image,
                                         window);
    }
}
