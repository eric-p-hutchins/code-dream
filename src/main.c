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
#include "code-dream-video-writer.h"
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
draw_to_renderer(code_dream_theme_t *theme,
                 code_dream_code_display_set_t *displays,
                 SDL_Renderer *renderer)
{
  SDL_SetRenderDrawColor(renderer,
                         theme->background_color.r,
                         theme->background_color.g,
                         theme->background_color.b,
                         255);
  SDL_RenderClear(renderer);
  code_dream_code_display_set_draw(displays, renderer);
}

void
draw(code_dream_theme_t *theme,
     code_dream_code_display_set_t *displays,
     code_dream_loading_screen_t *loading_screen,
     code_source_t *code_source,
     code_image_set_t *code_image_set,
     SDL_Window *window,
     code_dream_gif_writer_t *gif_writer,
     code_dream_video_writer_t *video_writer)
{
  SDL_Renderer *renderer = SDL_GetRenderer(window);
  if (code_source_loading(code_source)
      || code_image_set_loading(code_image_set))
    {
      code_dream_loading_screen_draw(loading_screen);
    }
  else
    {
      draw_to_renderer(theme, displays, renderer);
      if (gif_writer != NULL && displays->n_displays > 0)
        {
          draw_to_renderer(theme,
                           displays,
                           code_dream_gif_writer_get_renderer(gif_writer));
          code_dream_gif_writer_draw_frame(gif_writer, displays);
        }
      if (video_writer != NULL && displays->n_displays > 0)
        {
          draw_to_renderer(theme,
                           displays,
                           code_dream_video_writer_get_renderer(video_writer));
          code_dream_video_writer_write_frame(video_writer, displays);
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
      if (code_source->n_sets == 0)
        {
          fprintf(stderr,
                  "Error: No source code found at file or directory %s\n",
                  code_source->filename);
          code_source_destroy(code_source);
          SDL_Quit();
          exit(0);
        }
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
  options->theme = NULL;
  options->light = true;
  int i;
  for (i = 1; i < argc; ++i)
    {
      if (strcmp("-d", argv[i]) == 0
               || strcmp("--dark", argv[i]) == 0)
        {
          options->light = false;
        }
      else if (strcmp("-f", argv[i]) == 0
               || strcmp("--fullscreen", argv[i]) == 0)
        {
          options->fullscreen = true;
        }
      else if (parse_int_arg(argc, argv, "--height", "screen height", &i,
                             &options->screen_height))
        {
          continue;
        }
      else if (strcmp("-h", argv[i]) == 0
               || strcmp("--help", argv[i]) == 0)
        {
          options->help = true;
        }
      else if (parse_string_arg(argc, argv, "-o", "--output",
                                "output", &i,
                                &options->output))
        {
          if (!ends_with(options->output, ".gif")
              && !ends_with(options->output, ".mov"))
            {
              fprintf(stderr, "%s\n",
                      "warning: output ignored. Only GIF and MOV are supported.");
              free(options->output);
              options->output = NULL;
            }
          continue;
        }
      else if (parse_string_arg(argc, argv, "-t", "--theme",
                                "theme", &i,
                                &options->theme))
        {
          continue;
        }
      else if (strcmp("--version", argv[i]) == 0)
        {
          options->version = true;
        }
      else if (parse_int_arg(argc, argv, "--width", "screen width", &i,
                        &options->screen_width))
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
  "  -d, --dark                      use dark default emacs colors when\n"
  "                                    --theme is not in effect.\n"
  "  -f, --fullscreen                set fullscreen.\n"
  "      --height                    set screen height.\n"
  "  -h, --help                      display this help and exit.\n"
  "  -o, --output                    generate output file.\n"
  "                                    only GIF and MOV are currently\n"
  "                                    supported.\n"
  "      --version                   display version information and exit.\n"
  "      --width                     set screen width.\n"
  "  -t, --theme                     set theme.\n"
  "                                    this can be anything that emacs will\n"
  "                                    recognize with M-x load-theme\n"
  "  -x                              set screen x position.\n"
  "  -y                              set screen y position.\n"
  "\n"
  "The filename is the file that the code will be taken from.\n"
  "If filename is a directory then it will use any code files it\n"
  "finds in that directory (recursively).";

char *
get_basedir(const char *path)
{
  char *basedir = (char*)malloc(strlen(path));
  char *last = strrchr(path, '/');
  if (last == NULL)
    {
      if (strcmp(path, "code-dream") == 0)
        {
          free(basedir);
          return strdup(".");
        }
      fprintf(stderr, "Error parsing path %s\n", path);
      free(basedir);
      return NULL;
    }
  strncpy(basedir, path, last - path);
  basedir[last - path] = '\0';
  return basedir;
}

int
main (int argc, char *argv[])
{
  char *basedir = get_basedir(argv[0]);
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
  SDL_Init(SDL_INIT_VIDEO);
  code_source_t *code_source = code_source_create(basedir, options->filename);
  if (code_source == NULL)
    {
      return 0;
    }
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
  SDL_Renderer *renderer =
    SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
  SDL_Renderer *output_renderer = NULL;
  code_dream_theme_t *theme = code_dream_theme_create(options->theme,
                                                      options->light,
                                                      basedir);
  if (theme == NULL)
    {
      code_source_destroy(code_source);
      SDL_Quit();
      exit(0);
    }
  code_dream_gif_writer_t *gif_writer = NULL;
  code_dream_video_writer_t *video_writer = NULL;
  if (options->output != NULL)
    {
      if (ends_with(options->output, ".gif"))
        {
          gif_writer =
            code_dream_gif_writer_create(basedir,
                                         options->output,
                                         code_source,
                                         theme,
                                         options->screen_width,
                                         options->screen_height);
          output_renderer = code_dream_gif_writer_get_renderer(gif_writer);
        }
      if (ends_with(options->output, ".mov"))
        {
          video_writer =
            code_dream_video_writer_create(basedir,
                                           options->output,
                                           code_source,
                                           theme,
                                           options->screen_width,
                                           options->screen_height);
          output_renderer = code_dream_video_writer_get_renderer(video_writer);
        }
    }
  SDL_Renderer *gif_writer_renderer = NULL;
  SDL_Renderer *video_writer_renderer = NULL;
  code_image_set_t *code_image_set =
    code_image_set_create(basedir, code_source, theme,
                          renderer,
                          output_renderer,
                          NULL);
  if (code_image_set == NULL)
    {
      code_dream_theme_destroy(theme);
      code_source_destroy(code_source);
      SDL_Quit();
      exit(0);
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

  // Handle events initially out of the loop to let windows,
  // etc. initialize
  handle_events(&running);

  Uint32 start_time = SDL_GetTicks();
  Uint32 current_frame_time;
  Uint32 next_frame_time;
  while (running)
    {
      current_frame_time = SDL_GetTicks();
      next_frame_time = current_frame_time + 20;
      handle_events(&running);
      update(code_source,
             code_image_set,
             loading_screen,
             displays);
      draw(theme,
           displays,
           loading_screen,
           code_source,
           code_image_set,
           window,
           gif_writer,
           video_writer);
      Sint32 time_left = next_frame_time - SDL_GetTicks();
      if (time_left > 0)
        {
          // Wait 2 ms less than the time left because it might sleep
          // too long because of OS scheduling.
          //
          // Then just tight loop for the last 2 ms (or less)
          SDL_Delay(time_left - 2);
          while (!SDL_TICKS_PASSED(SDL_GetTicks(), next_frame_time))
            {
              ; // tight loop
            }
        }
      ++t;
    }

  if (options->output != NULL)
    {
      if (gif_writer != NULL)
        {
          code_dream_gif_writer_write_gif(gif_writer);
          code_dream_gif_writer_destroy(gif_writer);
        }
      if (video_writer != NULL)
        {
          code_dream_video_writer_finish(video_writer);
          code_dream_video_writer_destroy(video_writer);
        }
    }
  code_dream_theme_destroy(theme);
  code_dream_loading_screen_destroy(loading_screen);
  code_dream_code_display_set_destroy(displays);
  code_image_set_destroy(code_image_set);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
}
