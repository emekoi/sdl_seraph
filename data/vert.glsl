#version 120

attribute vec4 sr_Position;
attribute vec4 sr_TexCoord;

void main() {
  gl_Position = sr_Position;
  gl_TexCoord[0] = sr_TexCoord;
}
