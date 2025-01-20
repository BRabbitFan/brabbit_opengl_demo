#version 450 core

in vec3 normal;
in vec3 vertex_global_position;

out vec4 FragColor;

uniform vec4 object_color;
uniform vec4 light_color;
uniform vec3 light_position;

void main() {
  float ambient_strength = 0.1;
  vec3 ambient = ambient_strength * light_color.rgb;

  vec3 normal_vec = normalize(normal);
  vec3 light_dir = normalize(light_position - vertex_global_position);
  float diff = max(dot(normal_vec, light_dir), 0.0);
  vec3 diffuse = diff * light_color.rgb;

  vec3 result = (ambient + diffuse) * object_color.rgb;

  FragColor = vec4(result.rgb, 1.0);
}
