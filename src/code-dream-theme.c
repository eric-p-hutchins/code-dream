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

#include "code-dream-theme.h"
#include "config.h"

void
load_color(SDL_Color *color, FILE *file)
{
  char red[3] = "00";
  char green[3] = "00";
  char blue[3] = "00";
  fread(red, 1, 2, file);
  fread(green, 1, 2, file);
  fread(blue, 1, 2, file);
  color->r = strtol(red, NULL, 16);
  color->g = strtol(green, NULL, 16);
  color->b = strtol(blue, NULL, 16);
  color->a = 255;
}

code_dream_theme_t *
code_dream_theme_create(const char *name,
                        bool light,
                        const char *basedir)
{
  code_dream_theme_t *theme =
    (code_dream_theme_t*)malloc(sizeof(code_dream_theme_t));
  char *theme_el_path = (char*)malloc(strlen(basedir)
                                      + strlen("/theme.el")
                                      + 1);
  strcpy(theme_el_path, basedir);
  strcat(theme_el_path, "/theme.el");
  FILE *theme_el = fopen(theme_el_path, "r");
  if (theme_el == NULL)
    {
      free(theme_el_path);
      theme_el_path = (char*)malloc(strlen(basedir)
                                    + strlen("/../theme.el ")
                                    + 1);
      strcpy(theme_el_path, basedir);
      strcat(theme_el_path, "/../theme.el");
      theme_el = fopen(theme_el_path, "r");
    }
  if (theme_el == NULL)
    {
      free(theme_el_path);
      theme_el_path = (char*)malloc(strlen(DATADIR)
                                    + strlen("/theme.el")
                                    + 1);
      strcpy(theme_el_path, DATADIR);
      strcat(theme_el_path, "/theme.el");
      theme_el = fopen(theme_el_path, "r");
    }
  if (theme_el == NULL)
    {
      free(theme_el_path);
      free(theme);
      return NULL;
    }
  fclose(theme_el);
  char *command = (char*)malloc(strlen(EMACS)
                                + strlen(" --script ")
                                + strlen(theme_el_path)
                                + ((name == NULL && !light) ?
                                   strlen(" --dark") : 0)
                                + ((name != NULL) ?
                                   (strlen(" ") + strlen(name)) : 0)
                                + 1);
  strcpy(command, EMACS);
  strcat(command, "  --script ");
  strcat(command, theme_el_path);
  if (name == NULL && !light)
    {
      strcat(command, " --dark");
    }
  else if (name != NULL)
    {
      strcat(command, " ");
      strcat(command, name);
    }
  free(theme_el_path);
  FILE *file = popen(command, "r");
  if (file == NULL)
    {
      fprintf(stderr, "Failed to execute command\n");
      free(command);
      free(theme);
      return NULL;
    }
  free(command);
  load_color(&(theme->background_color), file);
  load_color(&(theme->default_color), file);
  load_color(&(theme->preproc_color), file);
  load_color(&(theme->string_color), file);
  load_color(&(theme->keyword_color), file);
  load_color(&(theme->type_color), file);
  load_color(&(theme->func_color), file);
  load_color(&(theme->var_color), file);
  load_color(&(theme->keyvalue_color), file);
  load_color(&(theme->comment_color), file);
  pclose(file);
  return theme;
}

void
code_dream_theme_destroy(code_dream_theme_t *theme)
{
  free(theme);
}
