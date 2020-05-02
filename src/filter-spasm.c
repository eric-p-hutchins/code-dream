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

#define SPASM_DIR_UP 0x0
#define SPASM_DIR_RIGHT 0x8
#define SPASM_DIR_DOWN 0x10
#define SPASM_DIR_LEFT 0x18

#define CAN_SPASM_SET 0x80
#define CAN_SPASM 0x40
#define SPASMING 0x20
#define SPASM_DIR 0x18
#define SPASM_TIME 0x7

uint8_t
spasm(SDL_FRect *rect, uint8_t state)
{
  if (!(state & CAN_SPASM_SET))
    {
      state |= CAN_SPASM_SET;
      state |= CAN_SPASM * (rand() % 16 == 0);
    }
  else if (state & CAN_SPASM)
    {
      if (!(state & SPASMING))
        {
          if (rand() % 64 == 0)
            {
              state |= SPASMING;
              state |= ((rand() % 4) << 3);
            }
        }
      else
        {
          switch(state & SPASM_DIR)
            {
            case SPASM_DIR_UP:
              rect->y -= 0.25 * rect->h;
              break;
            case SPASM_DIR_RIGHT:
              rect->x += 0.25 * rect->h;
              break;
            case SPASM_DIR_DOWN:
              rect->y += 0.25 * rect->h;
              break;
            case SPASM_DIR_LEFT:
              rect->x -= 0.25 * rect->h;
              break;
            }
          uint8_t time = state & SPASM_TIME;
          ++time;
          state = (state & (CAN_SPASM_SET | CAN_SPASM | SPASMING | SPASM_DIR))
            | (time & SPASM_TIME);
          if (time >= 3)
            {
              state = CAN_SPASM_SET | CAN_SPASM;
            }
        }
    }
  return state;
}
