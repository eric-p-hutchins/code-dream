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

#include "code-dream-smoke.h"

code_dream_smoke_t *
code_dream_smoke_create(double x,
                        double y,
                        double z,
                        double size)
{
  code_dream_smoke_t *smoke =
    (code_dream_smoke_t*)malloc(sizeof(code_dream_smoke_t));
  smoke->x = x;
  smoke->y = y;
  smoke->z = z;
  smoke->size = size;
  return smoke;
}

void
code_dream_smoke_draw(code_dream_smoke_t *smoke,
                      Uint8 *pixels,
                      int r,
                      int g,
                      int b,
                      int screen_width,
                      int screen_height)
{
  int x =
    screen_width / 2 + smoke->x / smoke->z * screen_height;
  int y =
    screen_height / 2 - smoke->y / smoke->z * screen_height;
  int size = smoke->size / smoke->z * screen_height;
  int min_x = x;
  int max_x = x + size;
  int min_y = y;
  int max_y = y + size;
  if (min_x < 0) min_x = 0;
  if (min_x > screen_width) min_x = screen_width;
  if (max_x < 0) max_x = 0;
  if (max_x > screen_width) max_x = screen_width;
  if (min_y < 0) min_y = 0;
  if (min_y > screen_height) min_y = screen_height;
  if (max_y < 0) max_y = 0;
  if (max_y > screen_height) max_y = screen_height;
  int i, j;
  for (j = min_x; j < max_x; ++j)
    {
      for (i = min_y; i < max_y; ++i)
        {
          Uint8* pixel =
            pixels + screen_width * 4 * i + j * 4;
          pixel[0] = r;
          pixel[1] = g;
          pixel[2] = b;
          Uint16 new_alpha = pixel[3] + 2;
          if (new_alpha > 255) new_alpha = 255;
          pixel[3] = new_alpha;
        }
    }
}

void
code_dream_smoke_destroy(code_dream_smoke_t *smoke)
{
  free(smoke);
}
