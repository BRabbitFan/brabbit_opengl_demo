#version 450 core

// get input into vec3(Type) aPos(name) in location 0
layout (location = 0) in vec3 vertex_position;

out vec4 vertex_color;

uniform mat4 transform;

void main() {
  // set output into gl_Position(pre defined variant)
  gl_Position = transform * vec4(vertex_position, 1.0f);

  vertex_color = vec4(vertex_position, 1.0f);
}
