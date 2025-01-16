#version 450 core

// get input into vec3(Type) aPos(name) in location 0
layout (location = 0) in vec3 vertex_position;

out vec4 vertex_color;

uniform mat4 transform;

void main() {
  bool x_max = abs(vertex_position.x) == 0.5;
  bool y_max = abs(vertex_position.y) == 0.5;
  bool z_max = abs(vertex_position.z) == 0.5;

  if (x_max && !y_max && !z_max) {
    vertex_color = vec4(1.0, 0.0, 0.0, 1.0);
  } else if (y_max && !x_max && !z_max) {
    vertex_color = vec4(0.0, 1.0, 0.0, 1.0);
  } else if (z_max && !x_max && !y_max) {
    vertex_color = vec4(0.0, 0.0, 1.0, 1.0);
  } else {
    vertex_color = vec4(0.0, 0.0, 0.0, 1.0);
  }

  // set output into gl_Position(pre defined variant)
  gl_Position = transform * vec4(vertex_position, 1.0);
}
