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

#include "code-dream-loading-screen.h"

code_dream_loading_screen_t *
code_dream_loading_screen_create(SDL_Renderer *renderer,
                                 int screen_width,
                                 int screen_height)
{
  code_dream_loading_screen_t *loading_screen =
    (code_dream_loading_screen_t *)malloc(sizeof(code_dream_loading_screen_t));
  loading_screen->renderer = renderer;
  loading_screen->screen_width = screen_width;
  loading_screen->screen_height = screen_height;
  return loading_screen;
}

void
code_dream_loading_screen_update(code_dream_loading_screen_t *loading_screen)
{
  ++loading_screen->t;
}

void
code_dream_loading_screen_draw_point_big(SDL_Renderer *renderer, int x, int y)
{
  SDL_RenderDrawPoint(renderer, x, y);
  SDL_RenderDrawPoint(renderer, x - 1, y);
  SDL_RenderDrawPoint(renderer, x + 1, y);
  SDL_RenderDrawPoint(renderer, x, y - 1);
  SDL_RenderDrawPoint(renderer, x, y + 1);
}

void
code_dream_loading_screen_draw(code_dream_loading_screen_t *loading_screen)
{
  int screen_width = loading_screen->screen_width;
  int screen_height = loading_screen->screen_height;
  int t = loading_screen->t;
  SDL_Renderer *renderer = loading_screen->renderer;

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 224, 224, 224, 255);
  int x = screen_width / 2 + 20 * cos(t / 8.0);
  int y = screen_height / 2 + 20 * sin(t / 8.0);
  code_dream_loading_screen_draw_point_big(renderer, x, y);
  SDL_SetRenderDrawColor(renderer, 160, 160, 160, 255);
  x = screen_width / 2 + 20 * cos(t / 8.0 - 0.3);
  y = screen_height / 2 + 20 * sin(t / 8.0 - 0.3);
  code_dream_loading_screen_draw_point_big(renderer, x, y);
  SDL_SetRenderDrawColor(renderer, 96, 96, 96, 255);
  x = screen_width / 2 + 20 * cos(t / 8.0 - 0.6);
  y = screen_height / 2 + 20 * sin(t / 8.0 - 0.6);
  code_dream_loading_screen_draw_point_big(renderer, x, y);
  SDL_SetRenderDrawColor(renderer, 32, 32, 32, 255);
  x = screen_width / 2 + 20 * cos(t / 8.0 - 0.9);
  y = screen_height / 2 + 20 * sin(t / 8.0 - 0.9);
  code_dream_loading_screen_draw_point_big(renderer, x, y);
}

void
code_dream_loading_screen_destroy(code_dream_loading_screen_t *loading_screen)
{
  free(loading_screen);
}
