/**
 * Copyright (c) 2017 emekoi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */


#ifndef M_FONT_H
#define M_FONT_H

#include "ttf.h"
#include "m_buffer.h"

typedef struct {
  ttf_Font *font;
} Font;

Font *font_fromFile(const char *filename, int fontsize);
Font *font_fromString(const char *data, int fontsize);
Font *font_fromEmbedded(int fontsize);
int font_destroy(Font *self);
sr_Buffer *font_render(Font *self, const char *str);
void sr_drawText(sr_Buffer *b, Font *font, sr_Pixel c, const char *str, int x, int y, sr_Transform *t);
int font_getWidth(Font *self, const char *str);
int font_getHeight(Font *self);
int font_getSize(Font *self);

#endif