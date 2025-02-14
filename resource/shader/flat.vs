#version 450 core

// get input into vec3(Type) aPos(name) in location 0
layout (location = 0) in vec3 vertex_position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
  gl_Position = projection * view * model * vec4(vertex_position, 1.0);
}
