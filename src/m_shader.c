/**
 * Copyright (c) 2017 emekoi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */


#include "m_shader.h"
#include "util.h"
#include "fs/fs.h"


static Shader *shader_new() {
  Shader *self = calloc(1, sizeof(*self));
  return self;
}


static GLuint compileShader(const char *name, const GLchar *src, GLint type) {
  /* Compile vertex shader */
  const GLuint self = glCreateShader(type);
  glShaderSource(self, 1, &src, NULL);
  glCompileShader(self); GLint status;
  glGetShaderiv(self, GL_COMPILE_STATUS, &status);
  char buffer[1024];
  glGetShaderInfoLog(self, 1024, NULL, buffer);
  if (status != GL_TRUE) ERROR("%s : %s\n", name, buffer);
  return self;
}


Shader *shader_fromString(const char *vertex, const char *v, const char *fragment, const char *f) {
  Shader *self = shader_new(); GLuint program;
  self->vertex = compileShader(v, vertex, GL_VERTEX_SHADER);
  self->fragment = compileShader(f, fragment, GL_FRAGMENT_SHADER);
  program =  glCreateProgram();
  glAttachShader(program, self->vertex);
  glAttachShader(program, self->fragment);
  glLinkProgram(program);
  self->program = program;
  return self;
}


Shader *shader_fromFile(const char *vertex, const char *fragment) {
  GLchar *v_src = fs_read(vertex, NULL);
  GLchar *f_src = fs_read(fragment, NULL);
  Shader *self = shader_fromString(v_src, vertex, f_src, fragment);
  free(v_src); free(f_src);
  return self;
}


void shader_setAttribute(Shader *self, const char *name, int size, int type, int norm, int s, void *p) {
  GLint attrib = glGetAttribLocation(self->program, name);
  glVertexAttribPointer(attrib, size, type, norm, s, p);
  glEnableVertexAttribArray(attrib);
}


// static void set_uniform(GLuint location, int size, int vec, int type) {
//   if (vec) {

//   } else {

//   }
// }


// void shader_setUniform(Shader *S, const char *name, void *data) {
//   GLint uniform = glGetUniformLocation(self->program, name);
//   set_uniform(uniform, 0, 0, 0);
//   // glUniform3f(uniform, sin(time) * 2, cos(time) * 2, tan(time) * 2);
// }


void shader_destroy(Shader *self) {
  if (self) {
    glDetachShader(self->program, self->vertex);
    glDetachShader(self->program, self->fragment);
    glDeleteShader(self->vertex);
    glDeleteShader(self->fragment);
    glDeleteProgram(self->program);
    free(self);
  }
}
