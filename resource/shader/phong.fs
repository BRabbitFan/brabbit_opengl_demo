#version 450 core

in vec3 normal;
in vec3 vertex_global_position;

out vec4 FragColor;

uniform float ambient_strength;
uniform float specular_strength;

uniform vec4 object_color;
uniform vec4 light_color;
uniform vec3 light_position;
uniform vec3 camera_position;

void main() {
  vec3 ambient = vec3(0.0, 0.0, 0.0);
  if (ambient_strength > 0.0) {
    ambient = ambient_strength * light_color.rgb;
  }

  vec3 normal_vec = normalize(normal);
  vec3 light_dir = normalize(light_position - vertex_global_position);
  float diff = max(dot(normal_vec, light_dir), 0.0);
  vec3 diffuse = diff * light_color.rgb;

  vec3 specular = vec3(0.0, 0.0, 0.0);
  if (ambient_strength > 0.0) {
    vec3 view_dir = normalize(camera_position - vertex_global_position);
    vec3 reflect_dir = reflect(-light_dir, normal_vec);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32);
    specular = specular_strength * spec * light_color.rgb;
  }

  vec3 result = (ambient + diffuse + specular) * object_color.rgb;

  FragColor = vec4(result.rgb, 1.0);
}
