/**
 * Copyright (c) 2017 emekoi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "m_font.h"
#include "fs/fs.h"
#include "sera/sera.h"

#define DEFAULT_FONTSIZE 14


static Font *newFont() {
  Font *self = calloc(1, sizeof(*self));
  return self;
}

static const char *loadFontFromMemory(
  Font *self, const void *data, int len, int ptsize
) {
  self->font = ttf_new(data, len);
  if (!self->font) {
    return "could not load font";
  }
  ttf_ptsize(self->font, ptsize);
  return NULL;
}


Font *font_fromFile(const char *filename, int fontsize) {
  fontsize = (fontsize ? fontsize : DEFAULT_FONTSIZE);
  Font *self = newFont();
  size_t len;
  void *data = fs_read(filename, &len);
  /* Load new font */
  if (!data) {
    CERROR("could not open file '%s'", filename);
  }
  const char *err = loadFontFromMemory(self, data, len, fontsize);
  free(data);
  if (err) CERROR("%s", err);
  return self;
}


Font *font_fromString(const char *data, int fontsize) {
  size_t len = strlen(data);
  fontsize = (fontsize ? fontsize : DEFAULT_FONTSIZE);
  Font *self = newFont();
  const char *err = loadFontFromMemory(self, data, len, fontsize);
  if (err) CERROR("%s", err);
  return self;
}


Font *font_fromEmbedded(int fontsize) {
  #include "default_ttf.h"
  fontsize = (fontsize ? fontsize : DEFAULT_FONTSIZE);
  Font *self = newFont();
  const char *err = loadFontFromMemory(self, default_ttf, sizeof(default_ttf),
                                       fontsize);
  if (err) CERROR("%s", err);
  return self;
}


int font_destroy(Font *self) {
  if (self && self->font) {
    ttf_destroy(self->font);
  }
  return 0;
}


sr_Buffer *font_render(Font *self, const char *str) {
  int w, h;
  if (!str || *str == '\0') str = " ";
  sr_Buffer *b;
  void *data = ttf_render(self->font, str, &w, &h);
  if (!data) CERROR("could not render text");
  /* Load bitmap and free intermediate 8bit bitmap */
  b = sr_newBufferBlank(w, h);
  if (!b) {
    free(data);
    CERROR("could not create buffer");
  }
  sr_loadPixels8(b, data, NULL);
  free(data);
  return b;
}


void sr_drawText(sr_Buffer *b, Font *font, sr_Pixel c, const char *str, int x, int y, sr_Transform *t) {
  sr_Buffer *txt = font_render(font, str); sr_setColor(b, c);
  sr_drawBuffer(b, txt, x, y, NULL, t);
  sr_reset(b); sr_destroyBuffer(txt);
}


int font_getWidth(Font *self, const char *str) {
  return ttf_width(self->font, str);
}


int font_getHeight(Font *self) {
  return ttf_height(self->font);
}


int font_getSize(Font *self) {
  return self->font->ptsize;
}