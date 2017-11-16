/**
 * Copyright (c) 2017 emekoi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */


#ifndef M_BUFFER_H
#define M_BUFFER_H

#include "sera/sera.h"

sr_Buffer *new_srBuffer(void);
sr_Buffer *sr_newBufferFile(const char *filename);
sr_Buffer *sr_newBufferString(const char *str);
sr_Buffer *sr_newBufferBlank(int w, int h);

#endif