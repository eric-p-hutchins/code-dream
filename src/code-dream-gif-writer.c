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

#include "code-dream-gif-writer.h"

code_dream_gif_writer_t *
code_dream_gif_writer_create(code_source_t *code_source,
                             int screen_width,
                             int screen_height)
{
  code_dream_gif_writer_t *writer =
    (code_dream_gif_writer_t*)malloc(sizeof(code_dream_gif_writer_t));
  writer->priv =
    (code_dream_gif_writer_priv_t*)malloc(sizeof(code_dream_gif_writer_priv_t));
  code_dream_gif_writer_priv_t *priv =
    (code_dream_gif_writer_priv_t*)writer->priv;
  priv->surface =
    SDL_CreateRGBSurfaceWithFormat(0,
                                   screen_width,
                                   screen_height,
                                   24,
                                   SDL_PIXELFORMAT_RGB24);
  priv->renderer = SDL_CreateSoftwareRenderer(priv->surface);
  priv->code_image_set = code_image_set_create(code_source,
                                               priv->renderer);
  if (priv->code_image_set == NULL)
    {
      code_dream_gif_writer_destroy(writer);
      return NULL;
    }
  while (code_image_set_loading(priv->code_image_set))
    {
      ;
    }

  int error;
  priv->gif_file = EGifOpenFileName("test.gif", false, &error);
  if (priv->gif_file == NULL)
    {
      fprintf (stderr, "error: %d\n", error);
      code_dream_gif_writer_destroy(writer);
      return NULL;
    }

  GifColorType *colors = malloc(sizeof(GifColorType) * 256);
  int i;
  for (i = 0; i < 256; ++i)
    {
      colors[i] = (GifColorType){i, i, i};
    }
  ColorMapObject *color_map = GifMakeMapObject(256, colors);

  priv->gif_file->SWidth = priv->surface->w;
  priv->gif_file->SHeight = priv->surface->h;
  priv->gif_file->SColorResolution = 24;
  priv->gif_file->SBackGroundColor = 0;
  priv->gif_file->SColorMap = color_map;
  priv->gif_file->ImageCount = 0;
  priv->gif_file->SavedImages = NULL;
  priv->gif_file->ExtensionBlockCount = 0;
  priv->gif_file->ExtensionBlocks = NULL;

  return writer;
}

SDL_Renderer *
code_dream_gif_writer_get_renderer(code_dream_gif_writer_t *writer)
{
  code_dream_gif_writer_priv_t *priv =
    (code_dream_gif_writer_priv_t*)writer->priv;
  return priv->renderer;
}

SDL_Surface *
code_dream_gif_writer_get_surface(code_dream_gif_writer_t *writer)
{
  code_dream_gif_writer_priv_t *priv =
    (code_dream_gif_writer_priv_t*)writer->priv;
  return priv->surface;
}

code_image_set_t *
code_dream_gif_writer_get_code_image_set(code_dream_gif_writer_t *writer)
{
  code_dream_gif_writer_priv_t *priv =
    (code_dream_gif_writer_priv_t*)writer->priv;
  return priv->code_image_set;
}

void
code_dream_gif_writer_draw_frame(code_dream_gif_writer_t *writer,
                                 code_dream_code_display_set_t *displays)
{
  code_dream_gif_writer_priv_t *priv =
    (code_dream_gif_writer_priv_t*)writer->priv;
  code_image_set_t *code_image_set = priv->code_image_set;
  SDL_Surface *surface = priv->surface;
  SDL_Renderer *renderer = priv->renderer;

  ++priv->gif_file->ImageCount;
  priv->gif_file->SavedImages =
    realloc(priv->gif_file->SavedImages,
            sizeof(SavedImage) * priv->gif_file->ImageCount);

  GifImageDesc imageDesc;
  imageDesc.Left = 0;
  imageDesc.Top = 0;
  imageDesc.Width = surface->w;
  imageDesc.Height = surface->h;
  imageDesc.Interlace = false;
  imageDesc.ColorMap = NULL;

  SavedImage image;
  image.ImageDesc = imageDesc;
  image.RasterBits =
    (GifByteType*)malloc(sizeof(GifByteType) * surface->w * surface->h);
  image.ExtensionBlockCount = 0;
  image.ExtensionBlocks = NULL;

  // Add the loop application control before the first image
  if (priv->gif_file->ImageCount == 1)
    {
      unsigned char loop_extension_block[19];
      loop_extension_block[0] = 'N';
      loop_extension_block[1] = 'E';
      loop_extension_block[2] = 'T';
      loop_extension_block[3] = 'S';
      loop_extension_block[4] = 'C';
      loop_extension_block[5] = 'A';
      loop_extension_block[6] = 'P';
      loop_extension_block[7] = 'E';
      loop_extension_block[8] = '2';
      loop_extension_block[9] = '.';
      loop_extension_block[10] = '0';
      GifAddExtensionBlock(&(image.ExtensionBlockCount),
                           &(image.ExtensionBlocks),
                           APPLICATION_EXT_FUNC_CODE,
                           11, loop_extension_block);

      unsigned char sub_block[3];
      int sub_block_len = 3;
      sub_block[0] = 1;
      sub_block[1] = 0xFF;
      sub_block[2] = 0xFF;
      GifAddExtensionBlock(&(image.ExtensionBlockCount),
                           &(image.ExtensionBlocks),
                           CONTINUE_EXT_FUNC_CODE,
                           3, sub_block);
    }


  SDL_SetRenderDrawColor(renderer, 46, 52, 53, 255);
  SDL_RenderClear(renderer);

  // Replace the code image set with the one created for the software
  // renderer
  code_image_set_t *old_cis = displays->code_image_set;
  displays->code_image_set = code_image_set;
  int i;
  for (i = 0; i < displays->n_displays; ++i)
    {
      displays->displays[i]->code_image_set = code_image_set;
    }

  code_dream_code_display_set_draw(displays, renderer);
  SDL_RenderPresent(renderer);

  // Put back the original code image set for drawing to the screen
  displays->code_image_set = old_cis;
  for (i = 0; i < displays->n_displays; ++i)
    {
      displays->displays[i]->code_image_set = old_cis;
    }

  int j;
  for (i = 0; i < surface->h; ++i)
    {
      for (j = 0; j < surface->w; ++j)
        {
          Uint8 *pixels = (Uint8*)surface->pixels;
          int r = pixels[i * surface->pitch + j * 3];
          int g = pixels[i * surface->pitch + j * 3 + 1];
          int b = pixels[i * surface->pitch + j * 3 + 2];
          int v = (r + g + b) / 3;
          image.RasterBits[i * surface->w + j] = v;
        }
    }

  priv->gif_file->SavedImages[priv->gif_file->ImageCount - 1] = image;

  GraphicsControlBlock graphics_control_block;
  graphics_control_block.DisposalMode = DISPOSAL_UNSPECIFIED;
  graphics_control_block.UserInputFlag = false;
  graphics_control_block.DelayTime = 2;
  graphics_control_block.TransparentColor = NO_TRANSPARENT_COLOR;
  EGifGCBToSavedExtension(&graphics_control_block,
                          priv->gif_file,
                          priv->gif_file->ImageCount - 1);
}

void
code_dream_gif_writer_write_gif(code_dream_gif_writer_t *writer)
{
  code_dream_gif_writer_priv_t *priv =
    (code_dream_gif_writer_priv_t*)writer->priv;
  int result = EGifSpew(priv->gif_file);
  if (result == GIF_ERROR)
    {
      fprintf (stderr, "error spewing GIF\n");
      fprintf (stderr, "error: %d\n", priv->gif_file->Error);
    }
  else
    {
      priv->gif_file = NULL;
    }
}

void
code_dream_gif_writer_destroy(code_dream_gif_writer_t *writer)
{
  code_dream_gif_writer_priv_t *priv =
    (code_dream_gif_writer_priv_t*)writer->priv;
  int error;
  if (priv->gif_file != NULL)
    {
      if (EGifCloseFile(priv->gif_file, &error) == GIF_ERROR)
        {
          fprintf(stderr, "error closing gif file. error code: %d\n", error);
        }
    }
  SDL_DestroyRenderer(priv->renderer);
  SDL_FreeSurface(priv->surface);
  code_image_set_destroy(priv->code_image_set);
  free(priv);
  free(writer);
}
