#version 450 core

in vec4 vertex_color;

out vec4 FragColor;

uniform vec4 global_color;

void main() {
  if (global_color.a > 0.0) {
    FragColor = global_color;
  } else {
    FragColor = vertex_color;
  }
}
