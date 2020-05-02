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

#include <stdint.h>

#include "SDL2/SDL_rect.h"

uint8_t
jiggle(SDL_FRect *rect, uint8_t state)
{
  rect->x += (rand() % 3 - 1) * 0.015625 * rect->h;
  rect->y += (rand() % 3 - 1) * 0.015625 * rect->h;
  return state;
}
