#include <brabbit/phong_shader.hpp>

namespace brabbit {

  using namespace std::string_view_literals;

  PhongShader::PhongShader() : Shader{ "phong.vs"sv, "phong.fs"sv } {}

  PhongShader::~PhongShader() = default;

  auto PhongShader::setModel(const glm::mat4& model) const -> void {
    setMat4("model"sv, model);
  }

  auto PhongShader::setView(const glm::mat4& view) const -> void {
    setMat4("view"sv, view);
  }

  auto PhongShader::setProjection(const glm::mat4& projection) const -> void {
    setMat4("projection"sv, projection);
  }

  auto PhongShader::setAmbientStrength(float ambient_strength) const -> void {
    setFloat("ambient_strength"sv, ambient_strength);
  }

  auto PhongShader::setSpecularStrength(float specular_strength) const -> void {
    setFloat("specular_strength"sv, specular_strength);
  }

  auto PhongShader::setObjectColor(const glm::vec4& color) const -> void {
    setVec4("object_color"sv, color);
  }

  auto PhongShader::setLightColor(const glm::vec4& color) const -> void {
    setVec4("light_color"sv, color);
  }

  auto PhongShader::setLightPosition(const glm::vec3& position) const -> void {
    setVec3("light_position"sv, position);
  }

  auto PhongShader::setCameraPosition(const glm::vec3& position) const -> void {
    setVec3("camera_position"sv, position);
  }

}  // namespace brabbit
