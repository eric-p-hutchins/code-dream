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
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "SDL2/SDL_thread.h"

#include "config.h"
#include "code-source.h"

void code_source_load_char_info_sets(code_source_t *code_source,
                                     const char *filename);

int code_source_load(void *data);

extern char *get_basedir(const char *path);

code_source_t *
code_source_create(const char *basedir, const char *filename)
{
  code_source_t *code_source = (code_source_t*)malloc(sizeof(code_source_t));
  code_source->filename = strdup(filename);
  code_source->sets = NULL;
  code_source->n_sets = 0;
  code_source->loaded = false;

  code_source->highlight_path = (char*)malloc(strlen(basedir)
                                              + strlen("/highlight.el")
                                              + 1);
  strcpy(code_source->highlight_path, basedir);
  strcat(code_source->highlight_path, "/highlight.el");
  FILE *file = fopen(code_source->highlight_path, "r");
  if (file == NULL)
    {
      free(code_source->highlight_path);
      code_source->highlight_path = (char*)malloc(strlen(basedir)
                                                  + strlen("/../highlight.el")
                                                  + 1);
      strcpy(code_source->highlight_path, basedir);
      strcat(code_source->highlight_path, "/../highlight.el");
      file = fopen(code_source->highlight_path, "r");
    }
  if (file == NULL)
    {
      free(code_source->highlight_path);
      code_source->highlight_path = (char*)malloc(strlen(DATADIR)
                                                  + strlen("/highlight.el")
                                                  + 1);
      strcpy(code_source->highlight_path, DATADIR);
      strcat(code_source->highlight_path, "/highlight.el");
      file = fopen(code_source->highlight_path, "r");
    }
  if (file == NULL)
    {
      fprintf(stderr, "%s\n", "Error: can't find highlight.el");
      code_source_destroy(code_source);
      return NULL;
    }
  fclose(file);

  SDL_Thread *thread =
    SDL_CreateThread(code_source_load, "code_source_load", code_source);
  SDL_DetachThread(thread);
  return code_source;
}

code_dream_format_type_t
code_source_emacs_type_to_code_dream_type(const char *emacs_type)
{
  if (strcmp(emacs_type, "font-lock-preprocessor-face") == 0)
    {
      return CODR_PREPROC;
    }
  else if (strcmp(emacs_type, "font-lock-string-face") == 0)
    {
      return CODR_STRING;
    }
  else if (strcmp(emacs_type, "font-lock-variable-name-face") == 0)
    {
      return CODR_IDENTIFIER;
    }
  else if (strcmp(emacs_type, "font-lock-type-face") == 0)
    {
      return CODR_TYPE;
    }
  else if (strcmp(emacs_type, "font-lock-function-name-face") == 0)
    {
      return CODR_FUNCTION;
    }
  else if (strcmp(emacs_type, "font-lock-keyword-face") == 0)
    {
      return CODR_KEYWORD;
    }
  else if (strcmp(emacs_type, "font-lock-constant-face") == 0)
    {
      return CODR_KEYVALUE;
    }
  else if (strcmp(emacs_type, "font-lock-comment-face") == 0)
    {
      return CODR_COMMENT;
    }
  else if (strcmp(emacs_type, "font-lock-comment-delimiter-face") == 0)
    {
      return CODR_COMMENT;
    }
  else if (strcmp(emacs_type, "font-lock-doc-face") == 0)
    {
      return CODR_STRING;
    }
  else if (strcmp(emacs_type, "c-annotation-face") == 0)
    {
      return CODR_KEYVALUE;
    }
  return CODR_NORMAL;
}

code_dream_char_info_set_t *
code_source_get_char_info_set(code_source_t *code_source,
                              const char *filename)
{
  code_dream_char_info_set_t *set = code_dream_char_info_set_create();
  char *command = (char*)malloc(strlen(EMACS)
                                + strlen(" --script ")
                                + strlen(code_source->highlight_path)
                                + strlen(" ")
                                + strlen(filename)
                                + strlen(" > ")
                                + strlen(filename)
                                + strlen(".txt")
                                + 1);
  strcpy(command, EMACS);
  strcat(command, " --script ");
  strcat(command, code_source->highlight_path);
  strcat(command, " ");
  strcat(command, filename);
  strcat(command, " > ");
  strcat(command, filename);
  strcat(command, ".txt");
  if (system(command) == -1)
    {
      free(command);
      fprintf(stderr, "Error running highlight.el to generate syntax highlight information");
      return set;
    }
  free(command);
  char *info_filename =
    (char*)malloc(strlen(filename) + strlen(".txt") + 1);
  strcpy(info_filename, filename);
  strcat(info_filename, ".txt");
  FILE *file = fopen(info_filename, "r");
  if (file != NULL)
    {
      int row = 1;
      int col = 0;
      char line[1024];
      while (fgets(line, 1024, file) != NULL)
        {
          char c = line[0];
          if (c == '\n')
            {
              // Read the corresponding 'nil'
              fgets(line, 1024, file);
              ++row;
              col = 0;
              continue;
            }
          if (c != ' ')
            {
              char *emacs_type = strdup(line + 2);
              emacs_type[strlen(emacs_type) - 1] = '\0';
              code_dream_format_type_t type =
                code_source_emacs_type_to_code_dream_type(emacs_type);
              free(emacs_type);
              code_dream_char_info_t *char_info =
                code_dream_char_info_create(c, type, row, col);
              code_dream_char_info_set_add_char(set, char_info);
            }
          ++col;
        }
      set->n_lines = row - 1;
      fclose(file);
    }
  else
    {
      fprintf(stderr, "Failed to open file %s\n", info_filename);
    }
  free(info_filename);
  if (set->n_lines == 0)
    {
      code_dream_char_info_set_destroy(set);
      return NULL;
    }
  return set;
}

void
code_source_add_char_info_set(code_source_t *code_source,
                              code_dream_char_info_set_t *char_info_set)
{
  ++code_source->n_sets;
  size_t size = sizeof(code_dream_char_info_set_t*) * code_source->n_sets;
  code_source->sets =
    (code_dream_char_info_set_t**)realloc(code_source->sets, size);
  code_source->sets[code_source->n_sets - 1] = char_info_set;
}

void
code_source_load_char_info_sets(code_source_t *code_source,
                                const char *filename)
{
  DIR *dir = opendir(filename);
  if (dir == NULL)
    {
      code_dream_char_info_set_t *char_info_set =
        code_source_get_char_info_set(code_source, filename);
      if (char_info_set != NULL)
        {
          code_source_add_char_info_set(code_source, char_info_set);
        }
      return;
    }

  struct dirent *ent = readdir(dir);
  while (ent != NULL)
    {
      if (ent->d_type == DT_DIR
          && strcmp(".", ent->d_name) != 0
          && strcmp("..", ent->d_name) != 0)
        {
          char *ent_path = (char*)malloc(strlen(filename)
                                         + strlen("/")
                                         + strlen(ent->d_name)
                                         + 1);
          strcpy(ent_path, filename);
          strcat(ent_path, "/");
          strcat(ent_path, ent->d_name);
          code_source_load_char_info_sets(code_source, ent_path);
        }
      else if (strlen(ent->d_name) > 2
               && ((ent->d_name[strlen(ent->d_name) - 1] == 'c'
                    && ent->d_name[strlen(ent->d_name) - 2] == '.')
                   || (ent->d_name[strlen(ent->d_name) - 1] == 'h'
                       && ent->d_name[strlen(ent->d_name) - 2] == '.')
                   || (strlen(ent->d_name) > 5
                       && ent->d_name[strlen(ent->d_name) - 1] == 'a'
                       && ent->d_name[strlen(ent->d_name) - 2] == 'v'
                       && ent->d_name[strlen(ent->d_name) - 3] == 'a'
                       && ent->d_name[strlen(ent->d_name) - 4] == 'j'
                       && ent->d_name[strlen(ent->d_name) - 5] == '.')))
        {
          char *ent_path = (char*)malloc(strlen(filename)
                                         + strlen("/")
                                         + strlen(ent->d_name)
                                         + 1);
          strcpy(ent_path, filename);
          strcat(ent_path, "/");
          strcat(ent_path, ent->d_name);
          code_source_load_char_info_sets(code_source, ent_path);
        }
      ent = readdir(dir);
    }
  closedir(dir);
}

bool
code_source_loading(code_source_t *code_source)
{
  return !code_source->loaded;
}

int
code_source_load(void *data)
{
  code_source_t *code_source = (code_source_t*)data;
  code_source_load_char_info_sets(code_source, code_source->filename);
  if (code_source->n_sets == 0)
    {
      fprintf(stderr,
              "Error: No source code found at file or directory %s\n",
              code_source->filename);
    }
  code_source->loaded = true;
  return 0;
}

void
code_source_get_char_info_sets(code_source_t *code_source,
                               code_dream_char_info_set_t ***sets_ptr,
                               size_t *n_sets_ptr)
{
  if (sets_ptr != NULL)
    {
      *sets_ptr = code_source->sets;
    }
  if (n_sets_ptr != NULL)
    {
      *n_sets_ptr = code_source->n_sets;
    }
}

void
code_source_destroy(code_source_t *code_source)
{
  int i;
  for (i = 0; i < code_source->n_sets; ++i)
    {
      code_dream_char_info_set_destroy(code_source->sets[i]);
    }
  free(code_source->filename);
  free(code_source->highlight_path);
  free(code_source);
}
