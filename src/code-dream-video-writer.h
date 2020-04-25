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

#ifndef CODE_DREAM_VIDEO_WRITER_H
#define CODE_DREAM_VIDEO_WRITER_H

#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/frame.h"
#include "libswscale/swscale.h"

#include "SDL2/SDL_render.h"
#include "SDL2/SDL_surface.h"

#include "code-image-set.h"
#include "code-dream-code-display-set.h"

typedef struct code_dream_video_writer_t code_dream_video_writer_t;

struct code_dream_video_writer_t
{
  void *priv;
};

typedef struct code_dream_video_writer_priv_t code_dream_video_writer_priv_t;

struct code_dream_video_writer_priv_t
{
  SDL_Renderer *renderer;
  SDL_Surface *surface;
  AVDictionary *avformat_dict;
  AVFormatContext *avformat_context;
  AVDictionary *avcodec_dict;
  AVCodecContext *avcodec_context;
  AVFrame *input_frame;
  AVFrame *output_frame;
  struct SwsContext *sws_context;
  AVStream *stream;
  int frame;
  AVOutputFormat *fmt;
  AVFormatContext *oc;
};

code_dream_video_writer_t *
code_dream_video_writer_create(const char *basedir,
                               const char *filename,
                               code_source_t *code_source,
                               code_dream_theme_t *theme,
                               int screen_width,
                               int screen_height);

SDL_Renderer *
code_dream_video_writer_get_renderer(code_dream_video_writer_t *writer);

void
code_dream_video_writer_write_frame(code_dream_video_writer_t *writer,
                                    code_dream_code_display_set_t *displays);

void code_dream_video_writer_finish(code_dream_video_writer_t *writer);

void code_dream_video_writer_destroy(code_dream_video_writer_t *writer);

#endif
