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
#include "code-dream-gif-writer.h"
#include "code-dream-image.h"
#include "code-dream-loading-screen.h"
#include "code-dream-options.h"
#include "code-image-set.h"
#include "code-source.h"
#include "config.h"

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
draw(code_dream_code_display_set_t *displays,
     code_dream_loading_screen_t *loading_screen,
     code_source_t *code_source,
     code_image_set_t *code_image_set,
     SDL_Window *window,
     code_dream_gif_writer_t *gif_writer)
{
  SDL_Renderer *renderer = SDL_GetRenderer(window);
  if (code_source_loading(code_source)
      || code_image_set_loading(code_image_set))
    {
      code_dream_loading_screen_draw(loading_screen);
    }
  else
    {
      SDL_SetRenderDrawColor(renderer, 46, 52, 53, 255);
      SDL_RenderClear(renderer);
      code_dream_code_display_set_draw(displays, renderer);
      if (gif_writer != NULL && displays->n_displays > 0)
        {
          code_dream_gif_writer_draw_frame(gif_writer, displays);
        }
    }

  SDL_RenderPresent(renderer);
}

void
update(code_source_t *code_source,
       code_image_set_t *code_image_set,
       code_dream_loading_screen_t *loading_screen,
       code_dream_code_display_set_t *displays)
{
  if (code_source_loading(code_source)
      || code_image_set_loading(code_image_set))
    {
      code_dream_loading_screen_update(loading_screen);
    }
  else
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
      code_dream_code_display_set_update(displays);
    }
}

bool
parse_int_arg(int argc,
              char *argv[],
              const char *option,
              const char *option_name,
              int *i,
              int *ptr)
{
  if (strcmp(option, argv[*i]) == 0)
    {
      ++*i;
      if (*i >= argc)
        {
          fprintf(stderr, "Error: no %s given\n", option_name);
        }
      char *endptr;
      long value = strtol(argv[*i], &endptr, 10);
      if (endptr[0] != '\0')
        {
          fprintf(stderr, "Error parsing %s: %s\n", option_name, argv[*i]);
        }
      else
        {
          *ptr = (int)value;
        }
      return true;
    }
  return false;
}

bool
parse_string_arg(int argc,
                 char *argv[],
                 const char *short_option,
                 const char *long_option,
                 const char *option_name,
                 int *i,
                 char **ptr)
{
  if ((short_option != NULL && strcmp(short_option, argv[*i]) == 0)
      || (long_option != NULL && strcmp(long_option, argv[*i]) == 0))
    {
      ++*i;
      if (*i >= argc)
        {
          fprintf(stderr, "Error: no %s given\n", option_name);
        }
      *ptr = strdup(argv[*i]);
      return true;
    }
  return false;
}

bool
ends_with(char *str, char *suffix)
{
  int suffix_len = strlen(suffix);
  int str_len = strlen(str);
  if (str_len < suffix_len)
    {
      return false;
    }
  int i;
  for (i = 0; i < suffix_len; ++i)
    {
      if (str[str_len - 1 - i] != suffix[suffix_len - 1 - i])
        {
          return false;
        }
    }
  return true;
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
  options->version = false;
  options->fullscreen = false;
  options->filename = NULL;
  options->help = false;
  options->output = NULL;
  int i;
  for (i = 1; i < argc; ++i)
    {
      if (parse_int_arg(argc, argv, "--width", "screen width", &i,
                        &options->screen_width))
        {
          continue;
        }
      else if (parse_int_arg(argc, argv, "--height", "screen height", &i,
                             &options->screen_height))
        {
          continue;
        }
      else if (parse_int_arg(argc, argv, "-x", "screen x position", &i,
                             &options->screen_x))
        {
          continue;
        }
      else if (parse_int_arg(argc, argv, "-y", "screen y position", &i,
                             &options->screen_y))
        {
          continue;
        }
      else if (strcmp("-f", argv[i]) == 0
               || strcmp("--fullscreen", argv[i]) == 0)
        {
          options->fullscreen = true;
        }
      else if (parse_string_arg(argc, argv, "-o", "--output",
                                "output", &i,
                                &options->output))
        {
          if (!ends_with(options->output, ".gif"))
            {
              fprintf(stderr, "%s\n",
                      "warning: output ignored. Only GIF is supported.");
              free(options->output);
              options->output = NULL;
            }
          continue;
        }
      else if (strcmp("-h", argv[i]) == 0
               || strcmp("--help", argv[i]) == 0)
        {
          options->help = true;
        }
      else if (strcmp("--version", argv[i]) == 0)
        {
          options->version = true;
        }
      else
        {
          if (options->filename == NULL)
            {
              options->filename = argv[i];
            }
        }
    }
  return options;
}

char *help_text = "Usage: code-dream [OPTION]... [filename]\n"
  "\n"
  "Display animated source code... for backgrounds and stuff.\n"
  "\n"
  "Options:\n"
  "  -f, --fullscreen                set fullscreen.\n"
  "      --height                    set screen height.\n"
  "  -h, --help                      display this help and exit.\n"
  "  -o, --output                    generate output file.\n"
  "                                    only GIF is currently supported.\n"
  "      --version                   display version information and exit.\n"
  "      --width                     set screen width.\n"
  "  -x                              set screen x position.\n"
  "  -y                              set screen y position.\n"
  "\n"
  "The filename is the file that the code will be taken from.\n"
  "If filename is a directory then it will use any code files it\n"
  "finds in that directory (recursively).";

int
main (int argc, char *argv[])
{
  code_dream_options_t *options = parse_args(argc, argv);
  if (options->help)
    {
      printf("%s\n", help_text);
      return 0;
    }
  if (options->version)
    {
      printf("Code Dream %s\n", PACKAGE_VERSION);
      return 0;
    }
  if (options->filename == NULL)
    {
      fprintf(stderr, "Error: no filename given\n");
      printf("%s\n", help_text);
      return 0;
    }
  code_source_t *code_source = code_source_create(options->filename);
  SDL_Init(SDL_INIT_VIDEO);
  TTF_Init();
  const char *title = "Code Dream";
  if (options->fullscreen)
    {
      SDL_SetRelativeMouseMode(SDL_TRUE);
    }
  SDL_Window *window = SDL_CreateWindow(title,
                                        options->screen_x,
                                        options->screen_y,
                                        options->screen_width,
                                        options->screen_height,
                                        (options->fullscreen
                                         ? SDL_WINDOW_FULLSCREEN : 0)
                                        | SDL_WINDOW_OPENGL
                                        | SDL_WINDOW_BORDERLESS);
  code_dream_gif_writer_t *gif_writer = NULL;
  SDL_Renderer *renderer =
    SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
  code_image_set_t *code_image_set =
    code_image_set_create(code_source, renderer);
  if (code_image_set == NULL)
    {
      SDL_Quit();
      exit(0);
    }
  if (options->output != NULL)
    {
      gif_writer =
        code_dream_gif_writer_create(options->output,
                                     code_source,
                                     options->screen_width,
                                     options->screen_height);
    }
  code_dream_code_display_set_t *displays =
    code_dream_code_display_set_create(code_source,
                                       code_image_set,
                                       options->screen_width,
                                       options->screen_height);

  code_dream_loading_screen_t *loading_screen =
    code_dream_loading_screen_create(renderer,
                                     options->screen_width,
                                     options->screen_height);

  int t = 0;

  bool running = true;
  while (running)
    {
      handle_events(&running);
      update(code_source,
             code_image_set,
             loading_screen,
             displays);
      draw(displays,
           loading_screen,
           code_source,
           code_image_set,
           window,
           gif_writer);
      SDL_Delay(20);
      ++t;
    }

  if (options->output != NULL)
    {
      code_dream_gif_writer_write_gif(gif_writer);
      code_dream_gif_writer_destroy(gif_writer);
    }
  code_dream_loading_screen_destroy(loading_screen);
  code_dream_code_display_set_destroy(displays);
  code_image_set_destroy(code_image_set);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
}
