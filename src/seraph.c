/**
 * Copyright (c) 2017 emekoi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */

// https://wiki.libsdl.org/SDL_GL_BindTexture
// http://www.sdltutorials.com/sdl-tip-sdl-surface-to-opengl-texture
// https://open.gl/textures

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "seraph.h"

float bx = 0, by = 0;
Font *font;
sr_Buffer *hello;
Shader *shader;
GLuint vao, vbo, ebo, tex;
SDL_Thread *ANIM;


void onInit() {
  hello = sr_newBufferFile("hello_world.png");
  sr_drawBox(hello, sr_color(0, 140, 140), 0, 0, 128, 128);
  font = font_fromEmbedded(16);
  graphics_setClearColor(sr_color(69, 100, 109));

  shader = shader_fromFile("vert.glsl", "frag.glsl");
  graphics_useShader(shader);
  srand(time_getNow());
  ANIM = SDL_CreateThread(titleAnim, NULL);
}

void onDraw() {
  /* Draw and reset the image buffer */
  sr_Transform t0 = sr_transform(0, 0, 0,
    m_graphics_width / hello->w, m_graphics_height / hello->h);
  sr_drawBuffer(m_graphics_buffer, hello, 0, 0, NULL, &t0);
  char buf[65]; sprintf(buf, "%d FPS", time_getFps());
  sr_drawText(m_graphics_buffer, font, sr_color(200, 200, 200), buf, 8, 6, NULL);

  int x = rand() % (m_graphics_buffer->w + 1);
  int y = rand() % (m_graphics_buffer->h + 1);
  int x1 = rand() % (m_graphics_buffer->w + 1);
  int y1 = rand() % (m_graphics_buffer->h + 1);

  sr_drawLine(m_graphics_buffer, sr_color(255, 2255, 255), x, y, x1, y1);

  GLint uniform = glGetUniformLocation(shader->program, "elapsed");
  glUniform1f(uniform, time_getTime());
}

void onQuit(void) {
  sr_destroyBuffer(hello);
  font_destroy(font);
  graphics_close();
  shader_destroy(shader);

  glDeleteBuffers(1, &ebo);
  glDeleteBuffers(1, &vbo);

  glDeleteVertexArrays(1, &vao);
  glDeleteTextures(1, &tex);

  SDL_KillThread(ANIM);
  SDL_Quit(); fs_deinit();
}

int main(int argc, char **argv) {
  TRACE("%d\n", glGetError());

  #if _WIN32
    setvbuf(stdout, NULL, _IONBF, BUFSIZ);
  #endif
  atexit(onQuit);
  __init();

  for (;;) {
    time_step();
    system_poll();
    __draw();
  }

  return 0;
}

void __init(void) {
  int width = 512, height = 512;
  graphics_init(width, height, "seraph", 0, 0);
  graphics_setMaxFps(60); fs_error(fs_mount("data"));
  // Create Vertex Array Object
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Create a Vertex Buffer Object and copy the vertex data to it
  glGenBuffers(1, &vbo);

  float vertices[] = {
  //  Position                Texcoords
  -1.0f,  1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 1.0f, 1.0f, // Top-left
   1.0f,  1.0f, 1.0f, 1.0f,   1.0f, 0.0f, 1.0f, 1.0f, // Top-right
   1.0f, -1.0f, 1.0f, 1.0f,   1.0f, 1.0f, 1.0f, 1.0f, // Bottom-right
  -1.0f, -1.0f, 1.0f, 1.0f,   0.0f, 1.0f, 1.0f, 1.0f, // Bottom-left
  };

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glGenBuffers(1, &ebo);

  GLuint elements[] = {
    0, 1, 2,
    2, 3, 0,
  };

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D, tex);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  onInit();
}

void __draw(void) {
  graphics_clear(); onDraw();
  sr_Buffer *b = m_graphics_buffer;

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, b->w, b->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, b->pixels);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

  SDL_GL_SwapBuffers();

  /* Wait for next frame */
  static double last = 0;
  double step = (1. / m_graphics_maxFps);
  double now = time_getTime();
  double wait = step - (now - last);
  last += step;
  if (wait > 0) {
    time_sleep(wait);
  } else {
    last = now;
  }
}
