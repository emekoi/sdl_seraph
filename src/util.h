/**
 * Copyright (c) 2017 emekoi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */


#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GLFW/glfw3.h>
#include "m_buffer.h"

#define ERROR(...) do { \
  fprintf(stderr, __VA_ARGS__); \
  fprintf(stderr, "\n"); \
  exit(EXIT_FAILURE); \
} while(0);

#define CERROR(...) do { \
  fprintf(stderr, "[CERROR]: %s:%d %s(): ", __FILE__, __LINE__, __func__); \
  fprintf(stderr, __VA_ARGS__); \
  fprintf(stderr, "\n"); \
  exit(EXIT_FAILURE); \
} while(0);

#define TRACE(...) do { \
  fprintf(stdout, "[TRACE]: %s:%d %s(): ", __FILE__, __LINE__, __func__); \
  fprintf(stdout, __VA_ARGS__); \
  fprintf(stdout, "\n"); \
} while(0);

#define UNUSED(x)       ((void) (x))
#define MIN(a, b)       ((b) < (a) ? (b) : (a))
#define MAX(a, b)       ((b) > (a) ? (b) : (a))
#define CLAMP(x, a, b)  (MAX(a, MIN(x, b)))
#define LERP(a, b, p)   ((a) + ((b) - (a)) * (p))

#define RECT(b) sr_rect(0, 0, b->w, b->h)

static inline void glfwERROR(int c, const char *err) {
  char *code;
  switch (c) {
    case GLFW_NOT_INITIALIZED:     code = "GLFW_NOT_INITIALIZED"; break;
    case GLFW_NO_CURRENT_CONTEXT:  code = "GLFW_NO_CURRENT_CONTEXT"; break;
    case GLFW_INVALID_ENUM:        code = "GLFW_INVALID_ENUM"; break;
    case GLFW_INVALID_VALUE:       code = "GLFW_INVALID_VALUE"; break;
    case GLFW_OUT_OF_MEMORY:       code = "GLFW_OUT_OF_MEMORY"; break;
    case GLFW_API_UNAVAILABLE:     code = "GLFW_API_UNAVAILABLE"; break;
    case GLFW_VERSION_UNAVAILABLE: code = "GLFW_VERSION_UNAVAILABLE"; break;
    case GLFW_PLATFORM_ERROR:      code = "GLFW_PLATFORM_ERROR"; break;
    case GLFW_FORMAT_UNAVAILABLE:  code = "GLFW_FORMAT_UNAVAILABLE"; break;
    case GLFW_NO_WINDOW_CONTEXT:   code = "GLFW_NO_WINDOW_CONTEXT"; break;
    default:                       code = "???";
  }
  ERROR("%s: %s", code, err);
}

#endif
