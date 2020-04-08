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

#include "code-source.h"

code_source_t *
code_source_create()
{
  code_source_t *code_source = (code_source_t*)malloc(sizeof(code_source_t));
  code_source->filename = "./main.c";
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
  return CODR_NORMAL;
}

code_dream_char_info_set_t *
code_source_get_char_info_set(code_source_t *code_source)
{
  code_dream_char_info_set_t *set = code_dream_char_info_set_create();
  char *command = (char*)malloc(strlen("./highlight.el ")
                                + strlen(code_source->filename)
                                + strlen(" > ")
                                + strlen(code_source->filename)
                                + strlen(".txt")
                                + 1);
  strcpy(command, "./highlight.el ");
  strcat(command, code_source->filename);
  strcat(command, " > ");
  strcat(command, code_source->filename);
  strcat(command, ".txt");
  if (system(command) == -1)
    {
      fprintf(stderr, "Error running highlight.el to generate syntax highlight information");
      return set;
    }
  char *filename =
    (char*)malloc(strlen(code_source->filename + strlen(".txt") + 1));
  strcpy(filename, code_source->filename);
  strcat(filename, ".txt");
  FILE *file = fopen(filename, "r");
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
    }
  fclose(file);
  return set;
}
