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

#include "code-dream-code-display-set.h"
#include "code-dream-image.h"
#include "code-dream-options.h"
#include "code-image-set.h"
#include "code-source.h"

int t = 0;

int total_time = 240;

int line_to_zoom_under = 100;

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
draw_loading(SDL_Window *window)
{
  int screen_width, screen_height;
  SDL_GetWindowSize(window, &screen_width, &screen_height);
  SDL_Renderer *renderer = SDL_GetRenderer(window);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 224, 224, 224, 255);
  int x = screen_width / 2 + 20 * cos(t / 8.0);
  int y = screen_height / 2 + 20 * sin(t / 8.0);
  draw_point_big(renderer, x, y);
  SDL_SetRenderDrawColor(renderer, 160, 160, 160, 255);
  x = screen_width / 2 + 20 * cos(t / 8.0 - 0.3);
  y = screen_height / 2 + 20 * sin(t / 8.0 - 0.3);
  draw_point_big(renderer, x, y);
  SDL_SetRenderDrawColor(renderer, 96, 96, 96, 255);
  x = screen_width / 2 + 20 * cos(t / 8.0 - 0.6);
  y = screen_height / 2 + 20 * sin(t / 8.0 - 0.6);
  draw_point_big(renderer, x, y);
  SDL_SetRenderDrawColor(renderer, 32, 32, 32, 255);
  x = screen_width / 2 + 20 * cos(t / 8.0 - 0.9);
  y = screen_height / 2 + 20 * sin(t / 8.0 - 0.9);
  draw_point_big(renderer, x, y);
}

void
draw(code_dream_code_display_set_t *displays,
     code_image_set_t *code_image_set,
     SDL_Window *window)
{
  SDL_Renderer *renderer = SDL_GetRenderer(window);
  if (code_image_set_loading(code_image_set))
    {
      draw_loading(window);
    }
  else
    {
      if (t % total_time == 0)
        {
          line_to_zoom_under = rand() % code_image_set->n_lines;
        }
      SDL_SetRenderDrawColor(renderer, 46, 52, 53, 255);
      SDL_RenderClear(renderer);
      code_dream_code_display_set_draw(displays, window);
    }

  SDL_RenderPresent(renderer);
}

code_dream_options_t *
parse_args(int argc, char *argv[])
{
  code_dream_options_t *options =
    (code_dream_options_t*)malloc(sizeof(code_dream_options_t));
  options->screen_width = 640;
  options->screen_height = 400;
  options->screen_x = SDL_WINDOWPOS_CENTERED;
  options->screen_y = SDL_WINDOWPOS_CENTERED;
  options->fullscreen = false;
  int i;
  for (i = 0; i < argc; ++i)
    {
      if (strcmp("-w", argv[i]) == 0)
        {
          ++i;
          char *endptr;
          long width = strtol(argv[i], &endptr, 10);
          if (endptr[0] != '\0')
            {
              fprintf(stderr, "Error parsing screen width: %s\n", argv[i]);
            }
          else
            {
              options->screen_width = (int)width;
            }
        }
      else if (strcmp("-h", argv[i]) == 0)
        {
          ++i;
          char *endptr;
          long height = strtol(argv[i], &endptr, 10);
          if (endptr[0] != '\0')
            {
              fprintf(stderr, "Error parsing screen height: %s\n", argv[i]);
            }
          else
            {
              options->screen_height = (int)height;
            }
        }
      else if (strcmp("-x", argv[i]) == 0)
        {
          ++i;
          char *endptr;
          long x = strtol(argv[i], &endptr, 10);
          if (endptr[0] != '\0')
            {
              fprintf(stderr, "Error parsing screen x position: %s\n", argv[i]);
            }
          else
            {
              options->screen_x = (int)x;
            }
        }
      else if (strcmp("-y", argv[i]) == 0)
        {
          ++i;
          char *endptr;
          long y = strtol(argv[i], &endptr, 10);
          if (endptr[0] != '\0')
            {
              fprintf(stderr, "Error parsing screen y position: %s\n", argv[i]);
            }
          else
            {
              options->screen_y = (int)y;
            }
        }
      else if (strcmp("-f", argv[i]) == 0)
        {
          options->fullscreen = true;
        }
    }
  return options;
}

int
main (int argc, char *argv[])
{
  code_dream_options_t *options = parse_args(argc, argv);
  code_source_t *code_source = code_source_create();
  SDL_Init(SDL_INIT_VIDEO);
  TTF_Init();
  const char *title = "Code Dream";
  if (options->fullscreen)
    {
      SDL_SetRelativeMouseMode(SDL_TRUE);
    }
  int screen_width = 640;
  int screen_height = 400;
  SDL_Window *window = SDL_CreateWindow(title,
                                        options->screen_x,
                                        options->screen_y,
                                        options->screen_width,
                                        options->screen_height,
                                        (options->fullscreen
                                         ? SDL_WINDOW_FULLSCREEN : 0)
                                        | SDL_WINDOW_OPENGL
                                        | SDL_WINDOW_BORDERLESS);
  SDL_Renderer *renderer =
    SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
  code_image_set_t *code_image_set =
    code_image_set_create(code_source, renderer);
  if (code_image_set == NULL)
    {
      SDL_Quit();
      exit(0);
    }
  code_dream_code_display_set_t *displays =
    code_dream_code_display_set_create(code_image_set);
  bool running = true;
  while (running)
    {
      handle_events(&running);
      if (!code_image_set_loading(code_image_set))
        {
          if (displays->n_displays == 0)
            {
              code_dream_code_display_set_add_display(displays);

              code_dream_code_display_set_add_display(displays);
              double dist_range =
                displays->displays[1]->max_dist +
                displays->displays[1]->min_dist;
              displays->displays[1]->dist =
                displays->displays[1]->min_dist
                + dist_range * 2 / 3.0;
              code_dream_code_display_set_add_display(displays);
              displays->displays[2]->dist =
                displays->displays[2]->min_dist
                + dist_range / 3.0;
            }
        }
      code_dream_code_display_set_update(displays);
      draw(displays, code_image_set, window);
      SDL_Delay(20);
      ++t;
    }
  SDL_Quit();
}
