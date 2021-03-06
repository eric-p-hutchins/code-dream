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

void
load_face(code_dream_face_t *face, FILE *file)
{
  load_color(&(face->color), file);
  char bold[2] = "N";
  fread(bold, 1, 1, file);
  if (bold[0] == 'N')
    {
      face->weight = CD_NORMAL;
    }
  else if (bold[0] == 'B')
    {
      face->weight = CD_BOLD;
    }
  else
    {
      fprintf(stderr, "Error parsing bold attribute from theme.el output: %c\n",
              bold[0]);
    }
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
  load_face(&(theme->default_face), file);
  load_face(&(theme->preproc_face), file);
  load_face(&(theme->string_face), file);
  load_face(&(theme->keyword_face), file);
  load_face(&(theme->type_face), file);
  load_face(&(theme->func_face), file);
  load_face(&(theme->var_face), file);
  load_face(&(theme->keyvalue_face), file);
  load_face(&(theme->comment_face), file);
  pclose(file);
  return theme;
}

code_dream_face_t
code_dream_theme_format_type_to_face(code_dream_theme_t *theme,
                                     code_dream_format_type_t type)
{
  code_dream_face_t face = theme->default_face;
  switch(type)
    {
    case CODR_PREPROC:
      face = theme->preproc_face;
      break;
    case CODR_STRING:
      face = theme->string_face;
      break;
    case CODR_KEYWORD:
      face = theme->keyword_face;
      break;
    case CODR_TYPE:
      face = theme->type_face;
      break;
    case CODR_FUNCTION:
      face = theme->func_face;
      break;
    case CODR_IDENTIFIER:
      face = theme->var_face;
      break;
    case CODR_KEYVALUE:
      face = theme->keyvalue_face;
      break;
    case CODR_COMMENT:
      face = theme->comment_face;
      break;
    default:
      face = theme->default_face;
      break;
    }
  return face;
}

void
code_dream_theme_destroy(code_dream_theme_t *theme)
{
  free(theme);
}
