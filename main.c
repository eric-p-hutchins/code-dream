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

#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "code-source.h"
#include "code-image-set.h"
#include "code-dream-image.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 400

int t = 0;

int total_time = 240;

int line_to_zoom_under = 100;

int n_lines;

void
handle_events(bool *running)
{
  SDL_Event event;
  while (SDL_PollEvent(&event))
    {
      switch (event.type) {
      case SDL_QUIT:
        *running = false;
        break;
      }
    }
}

void
draw_point_big(SDL_Renderer *renderer, int x, int y)
{
  SDL_RenderDrawPoint(renderer, x, y);
  SDL_RenderDrawPoint(renderer, x - 1, y);
  SDL_RenderDrawPoint(renderer, x + 1, y);
  SDL_RenderDrawPoint(renderer, x, y - 1);
  SDL_RenderDrawPoint(renderer, x, y + 1);
}

void
draw_image(code_image_set_t *code_image_set,
           code_dream_image_t *image,
           SDL_Renderer *renderer)
{
  double min_dist = code_image_set->font_width / 1000.0;
  double max_dist = code_image_set->font_width / 10.0;
  double speed = (max_dist - min_dist) / (double)total_time;
  int t2 = t % total_time;
  double dist = max_dist - t2 * speed;
  double code_width = code_image_set->font_width * 80;
  int zoom_y = code_image_set->font_height * line_to_zoom_under;
  SDL_Rect rect;
  rect.x = SCREEN_WIDTH / 2 + (-code_width / 2 + image->x) / dist;
  rect.y = SCREEN_HEIGHT / 2 + (-zoom_y + image->y) / dist;
  rect.x += rand() % 3 + 1;
  rect.y += rand() % 3 + 1;
  rect.w = image->w / dist;
  rect.h = image->h / dist;
  SDL_RenderCopy(renderer, image->image, NULL, &rect);
}

void
draw_lines(code_image_set_t *code_image_set,
           code_dream_image_t **images,
           SDL_Renderer *renderer)
{
  int i = 0;
  for (i = 0; i < code_image_set->n_images; ++i)
    {
      draw_image(code_image_set, images[i], renderer);
    }
}

void
draw_loading(SDL_Renderer *renderer)
{
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 224, 224, 224, 255);
  int x = SCREEN_WIDTH / 2 + 20 * cos(t / 8.0);
  int y = SCREEN_HEIGHT / 2 + 20 * sin(t / 8.0);
  draw_point_big(renderer, x, y);
  SDL_SetRenderDrawColor(renderer, 160, 160, 160, 255);
  x = SCREEN_WIDTH / 2 + 20 * cos(t / 8.0 - 0.3);
  y = SCREEN_HEIGHT / 2 + 20 * sin(t / 8.0 - 0.3);
  draw_point_big(renderer, x, y);
  SDL_SetRenderDrawColor(renderer, 96, 96, 96, 255);
  x = SCREEN_WIDTH / 2 + 20 * cos(t / 8.0 - 0.6);
  y = SCREEN_HEIGHT / 2 + 20 * sin(t / 8.0 - 0.6);
  draw_point_big(renderer, x, y);
  SDL_SetRenderDrawColor(renderer, 32, 32, 32, 255);
  x = SCREEN_WIDTH / 2 + 20 * cos(t / 8.0 - 0.9);
  y = SCREEN_HEIGHT / 2 + 20 * sin(t / 8.0 - 0.9);
  draw_point_big(renderer, x, y);
}

void
draw(code_image_set_t *code_image_set, SDL_Renderer *renderer)
{
  if (code_image_set_loading(code_image_set))
    {
      draw_loading(renderer);
    }
  else
    {
      if (t % total_time == 0)
        {
          line_to_zoom_under = rand() % code_image_set->n_lines;
        }
      SDL_SetRenderDrawColor(renderer, 46, 52, 53, 255);
      SDL_RenderClear(renderer);
      draw_lines(code_image_set, code_image_set->images, renderer);
    }

  SDL_RenderPresent(renderer);
}

int
main (int argc, char *argv[])
{
  code_source_t *code_source = code_source_create();
  SDL_Init(SDL_INIT_VIDEO);
  TTF_Init();
  const char *title = "Code Dream";
  bool fullscreen = false;
  if (fullscreen)
    {
      SDL_SetRelativeMouseMode(SDL_TRUE);
    }
  SDL_Window *window = SDL_CreateWindow(title,
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        SCREEN_WIDTH, SCREEN_HEIGHT,
                                        (fullscreen ? SDL_WINDOW_FULLSCREEN : 0)
                                        | SDL_WINDOW_OPENGL);
  SDL_Renderer *renderer =
    SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
  code_image_set_t *code_image_set =
    code_image_set_create(code_source, renderer);
  if (code_image_set == NULL)
    {
      SDL_Quit();
      exit(0);
    }
  bool running = true;
  while (running)
    {
      handle_events(&running);
      draw(code_image_set, renderer);
      SDL_Delay(20);
      ++t;
    }
  SDL_Quit();
}
