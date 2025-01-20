#version 450 core

out vec4 FragColor;

uniform vec4 object_color;
uniform vec4 light_color;

void main() {
  float ambient_strength = 0.1;
  vec3 ambient = ambient_strength * light_color.rgb;
  vec3 result = ambient * object_color.rgb;

  // FragColor = object_color * light_color;
  FragColor = vec4(result.rgb, 1.0);
}
