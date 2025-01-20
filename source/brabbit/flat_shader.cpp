#include <brabbit/flat_shader.hpp>

namespace brabbit {

  using namespace std::string_view_literals;

  FlatShader::FlatShader() : Shader{ "flat.vs"sv, "flat.fs"sv } {}

  FlatShader::~FlatShader() {}

  auto FlatShader::setModel(const glm::mat4& model) const -> void {
    setMat4("model"sv, model);
  }

  auto FlatShader::setView(const glm::mat4& view) const -> void {
    setMat4("view"sv, view);
  }

  auto FlatShader::setProjection(const glm::mat4& projection) const -> void {
    setMat4("projection"sv, projection);
  }

  auto FlatShader::setLightColor(const glm::vec4& color) const -> void {
    setVec4("light_color"sv, color);
  }

}  // namespace brabbit
