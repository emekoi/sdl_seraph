#version 120

attribute vec4 sr_Vertex;
attribute vec4 sr_TexCoord;

void main() {
  gl_Position = sr_Vertex;
  gl_TexCoord[0] = sr_TexCoord;
}
