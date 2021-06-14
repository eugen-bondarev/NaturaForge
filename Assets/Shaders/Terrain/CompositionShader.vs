#version 440 core

layout (location = 0) in vec2 in_Position;
layout (location = 1) in vec2 in_UVs;

out vec2 out_UVs;

void main() {
  gl_Position = vec4(in_Position, 0.0, 1.0);
  out_UVs = in_UVs;
}