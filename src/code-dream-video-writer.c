#include "code-dream-video-writer.h"

code_dream_video_writer_t *
code_dream_video_writer_create(const char *basedir,
                               const char *filename,
                               code_source_t *code_source,
                               code_dream_theme_t *theme,
                               int screen_width,
                               int screen_height)
{
  code_dream_video_writer_t *writer =
    (code_dream_video_writer_t*)malloc(sizeof(code_dream_video_writer_t));
  writer->priv =
    (code_dream_video_writer_priv_t*)malloc(sizeof(code_dream_video_writer_priv_t));
  code_dream_video_writer_priv_t *priv =
    (code_dream_video_writer_priv_t*)writer->priv;
  priv->surface =
    SDL_CreateRGBSurfaceWithFormat(0,
                                   screen_width,
                                   screen_height,
                                   24,
                                   SDL_PIXELFORMAT_RGB24);
  priv->renderer = SDL_CreateSoftwareRenderer(priv->surface);
  priv->code_image_set = code_image_set_create(basedir,
                                               code_source,
                                               theme,
                                               priv->renderer);
  if (priv->code_image_set == NULL)
    {
      code_dream_video_writer_destroy(writer);
      return NULL;
    }
  while (code_image_set_loading(priv->code_image_set))
    {
      ;
    }
  priv->avformat_context = avformat_alloc_context();
  if (priv->avformat_context == NULL)
    {
      fprintf(stderr, "Error allocating format context\n");
      code_dream_video_writer_destroy(writer);
      return NULL;
    }

  priv->avformat_context->oformat = av_guess_format("mov", NULL, NULL);

  enum AVCodecID codec_id = priv->avformat_context->oformat->video_codec;
  AVCodec *codec = avcodec_find_encoder(codec_id);
  if (codec == NULL)
    {
      fprintf(stderr, "Error finding codec for encoding\n");
      code_dream_video_writer_destroy(writer);
      return NULL;
    }

  priv->stream = avformat_new_stream(priv->avformat_context, codec);
  priv->stream->id = priv->avformat_context->nb_streams - 1;

  priv->avcodec_context = avcodec_alloc_context3(codec);
  if (priv->avcodec_context == NULL) {
    fprintf(stderr, "Could not alloc an encoding context\n");
    exit(1);
  }

  priv->avcodec_context->codec_id = codec->id;

  priv->avcodec_context->bit_rate = 10000000;

  priv->avcodec_context->width = screen_width;
  priv->avcodec_context->height = screen_height;

  priv->stream->time_base = (AVRational){ 1, 50 };
  priv->avcodec_context->time_base = priv->stream->time_base;

  priv->avcodec_context->gop_size = 12;
  priv->avcodec_context->pix_fmt = AV_PIX_FMT_YUV420P;

  if (priv->avformat_context->oformat->flags & AVFMT_GLOBALHEADER)
    priv->avcodec_context->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

  int ret = avcodec_open2(priv->avcodec_context, codec, NULL);
  if (ret < 0)
    {
      fprintf(stderr, "Could not open video codec: %s\n", av_err2str(ret));
      exit(1);
    }

  priv->avformat_dict = NULL;

  priv->output_frame = av_frame_alloc();
  priv->output_frame->format = AV_PIX_FMT_YUV420P;
  priv->output_frame->width = screen_width;
  priv->output_frame->height = screen_height;
  if (av_frame_get_buffer(priv->output_frame, 0) < 0)
    {
      fprintf(stderr, "Error creating buffer for output frame\n");
    }

  priv->input_frame = av_frame_alloc();
  priv->input_frame->format = AV_PIX_FMT_RGB24;
  priv->input_frame->width = screen_width;
  priv->input_frame->height = screen_height;
  if (av_frame_get_buffer(priv->input_frame, 0) < 0)
    {
      fprintf(stderr, "Error creating buffer for input frame\n");
    }

  ret = avcodec_parameters_from_context(priv->stream->codecpar, priv->avcodec_context);
  if (ret < 0)
    {
      fprintf(stderr, "Could not copy the stream parameters\n");
      exit(1);
    }

  priv->sws_context = sws_getContext(priv->avcodec_context->width,
                                     priv->avcodec_context->height,
                                     AV_PIX_FMT_RGB24,
                                     priv->avcodec_context->width,
                                     priv->avcodec_context->height,
                                     priv->avcodec_context->pix_fmt,
                                     SWS_BICUBIC, NULL, NULL, NULL);
  if (priv->sws_context == NULL)
    {
      fprintf(stderr, "Could not initialize the conversion context\n");
      exit(1);
    }

  if (avio_open(&(priv->avformat_context->pb), filename, AVIO_FLAG_WRITE) < 0)
    {
      fprintf(stderr, "Error opening file %s\n", filename);
      code_dream_video_writer_destroy(writer);
      return NULL;
    }

  if (avformat_write_header(priv->avformat_context, &(priv->avformat_dict)) < 0)
    {
      fprintf(stderr, "Error initializing output stream\n");
      code_dream_video_writer_destroy(writer);
      return NULL;
    }
  priv->frame = 0;
  return writer;
}

void
code_dream_video_writer_write_frame(code_dream_video_writer_t *writer,
                                    code_dream_code_display_set_t *displays)
{
  code_dream_video_writer_priv_t *priv =
    (code_dream_video_writer_priv_t*)writer->priv;

  SDL_SetRenderDrawColor(priv->renderer,
                         priv->code_image_set->theme->background_color.r,
                         priv->code_image_set->theme->background_color.g,
                         priv->code_image_set->theme->background_color.b, 255);
  SDL_RenderClear(priv->renderer);

  // Replace the code image set with the one created for the software
  // renderer
  code_image_set_t *old_cis = displays->code_image_set;
  displays->code_image_set = priv->code_image_set;
  int i, j;
  for (i = 0; i < displays->n_displays; ++i)
    {
      displays->displays[i]->code_image_set = priv->code_image_set;
    }

  code_dream_code_display_set_draw(displays, priv->renderer);
  SDL_RenderPresent(priv->renderer);

  // Put back the original code image set for drawing to the screen
  displays->code_image_set = old_cis;
  for (i = 0; i < displays->n_displays; ++i)
    {
      displays->displays[i]->code_image_set = old_cis;
    }

  for (i = 0; i < priv->surface->h; ++i)
    {
      memcpy(priv->input_frame->data[0] + i * priv->surface->w * 3,
             ((Uint8*)priv->surface->pixels) + i * priv->surface->pitch,
             priv->surface->w * 3);
    }
  sws_scale(priv->sws_context,
            (const uint8_t * const *) priv->input_frame->data,
            priv->input_frame->linesize, 0,
            priv->avcodec_context->height, priv->output_frame->data,
            priv->output_frame->linesize);

  priv->output_frame->pts = priv->frame++;

  int got_packet = 0;
  AVPacket pkt = { 0 };
  av_init_packet(&pkt);
  int ret = avcodec_encode_video2(priv->avcodec_context, &pkt, priv->output_frame, &got_packet);

  if (ret < 0)
    {
      fprintf(stderr, "Error encoding video frame: %s\n", av_err2str(ret));
    }
  if (got_packet)
    {
      av_packet_rescale_ts(&pkt, priv->avcodec_context->time_base, priv->stream->time_base);
      pkt.stream_index = priv->stream->index;
      ret = av_interleaved_write_frame(priv->avformat_context, &pkt);
    }
}

void
code_dream_video_writer_finish(code_dream_video_writer_t *writer)
{
  code_dream_video_writer_priv_t *priv =
    (code_dream_video_writer_priv_t*)writer->priv;
  int got_packet = 0;
  AVPacket pkt = { 0 };
  av_init_packet(&pkt);

  // Pass NULL instead of a frame to tell it to flush the content
  int ret = avcodec_encode_video2(priv->avcodec_context, &pkt, NULL, &got_packet);
  while (got_packet)
    {
      // Keep writing packets as long as we get them from the encoder

      av_packet_rescale_ts(&pkt,
                           priv->avcodec_context->time_base,
                           priv->stream->time_base);
      pkt.stream_index = priv->stream->index;
      av_interleaved_write_frame(priv->avformat_context, &pkt);

      // Do it again...

      got_packet = 0;
      pkt = (AVPacket){ 0 };
      av_init_packet(&pkt);

      // Pass NULL instead of a frame to tell it to flush the content
      avcodec_encode_video2(priv->avcodec_context, &pkt, NULL, &got_packet);
    }

  if (av_write_trailer(priv->avformat_context) != 0)
    {
      fprintf(stderr, "Error writing trailer\n");
    }
}

void
code_dream_video_writer_destroy(code_dream_video_writer_t *writer)
{
  code_dream_video_writer_priv_t *priv =
    (code_dream_video_writer_priv_t*)writer->priv;
  if (priv->avcodec_context != NULL)
    {
      if (avcodec_is_open(priv->avcodec_context))
        {
          avcodec_close(priv->avcodec_context);
        }
      avcodec_free_context(&priv->avcodec_context);
    }
  av_frame_free(&(priv->output_frame));
  av_frame_free(&(priv->input_frame));
  sws_freeContext(priv->sws_context);
  avio_closep(&(priv->avformat_context->pb));
  if (priv->avformat_context != NULL)
    {
      avformat_free_context(priv->avformat_context);
    }
  SDL_DestroyRenderer(priv->renderer);
  SDL_FreeSurface(priv->surface);
  code_image_set_destroy(priv->code_image_set);
  free(priv);
  free(writer);
}
