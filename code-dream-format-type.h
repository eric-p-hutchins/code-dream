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

#ifndef CODE_DREAM_FORMAT_TYPE_H
#define CODE_DREAM_FORMAT_TYPE_H

typedef enum
  {
   CODR_NORMAL,
   CODR_PREPROC,
   CODR_STRING,
   CODR_KEYWORD,
   CODR_TYPE,
   CODR_FUNCTION,
   CODR_IDENTIFIER,
   CODR_KEYVALUE
  } code_dream_format_type_t;

#endif
