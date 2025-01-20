#version 450 core

// get input into vec3(Type) aPos(name) in location 0
layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;

out vec3 vertex_global_position;
out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
  // set output into gl_Position(pre defined variant)
  gl_Position = projection * view * model * vec4(vertex_position, 1.0);

  vertex_global_position = vec3(model * vec4(vertex_position, 1.0));
  normal = mat3(transpose(inverse(model))) * vertex_normal;
}
