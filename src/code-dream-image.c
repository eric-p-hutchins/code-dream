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

#include "code-dream-image.h"

code_dream_image_t *
code_dream_image_create(char c,
                        code_dream_face_t face,
                        SDL_Texture *image,
                        int x,
                        int y,
                        int w,
                        int h)
{
  code_dream_image_t *code_dream_image =
    (code_dream_image_t*)malloc(sizeof(code_dream_image_t));
  code_dream_image->c = c;
  code_dream_image->face = face;
  code_dream_image->image = image;
  code_dream_image->x = x;
  code_dream_image->y = y;
  code_dream_image->w = w;
  code_dream_image->h = h;
  return code_dream_image;
}

void
code_dream_image_destroy(code_dream_image_t *image)
{
  SDL_DestroyTexture(image->image);
  free(image);
}
