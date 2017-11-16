/**
 * Copyright (c) 2017 emekoi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */


#ifndef M_SERAPH_H
#define M_SERAPH_H

#include <SDL/SDL.h>
#include "glew/glew.h"
#include "fs/fs.h"

#include "util.h"

#include "m_time.h"
#include "m_font.h"
#include "m_system.h"
#include "m_buffer.h"
#include "m_shader.h"
#include "m_graphics.h"

void __init(void);
void __draw(void);

void onInit();
void onDraw();

#endif
