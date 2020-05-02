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

#define CAN_FLICKER_SET 0x80
#define CAN_FLICKER 0x40
#define FLICKERING 0x20

uint8_t
flicker(SDL_FRect *rect, uint8_t state)
{
  uint8_t can_flicker_set = state & CAN_FLICKER_SET;
  uint8_t can_flicker = state & CAN_FLICKER;
  uint8_t flickering = state & FLICKERING;
  if (!can_flicker_set)
    {
      state |= CAN_FLICKER * (rand() % 64 == 0);
      state |= CAN_FLICKER_SET;
    }
  else if (can_flicker)
    {
      if (!flickering)
        {
          if (rand() % 64 == 0)
            {
              state |= FLICKERING;
              rect->w = 0;
              rect->h = 0;
            }
        }
      else
        {
          rect->w = 0;
          rect->h = 0;
          uint8_t time = state & 0x1F;
          ++time;
          state = (state & (CAN_FLICKER_SET | CAN_FLICKER | FLICKERING)) | (time & 0x1F);
          if (time >= 3)
            {
              state &= (CAN_FLICKER_SET | CAN_FLICKER);
            }
        }
    }
  return state;
}
