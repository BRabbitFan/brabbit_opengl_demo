#include "shader.hpp"
#include <filesystem>
#include <fstream>
#include <string>
#include <string_view>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader.hpp>

namespace brabbit {

  using namespace std::string_literals;
  using namespace std::string_view_literals;

  namespace {

    auto LoadShaderSource(std::string_view name) -> std::string {
      auto path = std::filesystem::current_path() / "resource"sv / "shader"sv / name;
      if (!std::filesystem::exists(path)) {
        return {};
      }

      auto file = std::ifstream{ path };
      if (!file) {
        return {};
      }

      return { std::istreambuf_iterator<char>{ file }, std::istreambuf_iterator<char>{} };
    }

  }  // namespace

  Shader::Shader(std::string_view vertex_name, std::string_view fragment_name) {
    id_ = glCreateProgram();

    auto vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    auto vertex_shader_source = LoadShaderSource(vertex_name);
    auto vertex_shader_source_cstr = vertex_shader_source.c_str();
    glShaderSource(vertex_shader, 1, &vertex_shader_source_cstr, NULL);
    glCompileShader(vertex_shader);
    glAttachShader(id_, vertex_shader);

    auto fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    auto fragment_shader_source = LoadShaderSource(fragment_name);
    auto fragment_shader_source_cstr = fragment_shader_source.c_str();
    glShaderSource(fragment_shader, 1, &fragment_shader_source_cstr, NULL);
    glCompileShader(fragment_shader);
    glAttachShader(id_, fragment_shader);

    glLinkProgram(id_);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
  }

  auto Shader::getId() const -> GLuint {
    return id_;
  }

  auto Shader::use() -> void {
    glUseProgram(id_);
  }

  auto Shader::getLocation(std::string_view name) const -> GLint {
    return glGetUniformLocation(id_, name.data());
  }

  auto Shader::setFloat(std::string_view name, GLfloat value) const -> void {
    glUniform1f(getLocation(name), value);
  }

  auto Shader::setInt(std::string_view name, GLint value) const -> void {
    glUniform1i(getLocation(name), value);
  }

  auto Shader::setUint(std::string_view name, GLuint value) const -> void {
    glUniform1ui(getLocation(name), value);
  }

  auto Shader::setVec2(std::string_view name, const glm::vec2& value) const -> void {
    glUniform2f(getLocation(name), value.x, value.y);
  }

  auto Shader::setVec3(std::string_view name, const glm::vec3& value) const -> void {
    glUniform3f(getLocation(name), value.x, value.y, value.z);
  }

  auto Shader::setVec4(std::string_view name, const glm::vec4& value) const -> void {
    glUniform4f(getLocation(name), value.x, value.y, value.z, value.w);
  }

  auto Shader::setIvec2(std::string_view name, const glm::ivec2& value) const -> void {
    glUniform2i(getLocation(name), value.x, value.y);
  }

  auto Shader::setIvec3(std::string_view name, const glm::ivec3& value) const -> void {
    glUniform3i(getLocation(name), value.x, value.y, value.z);
  }

  auto Shader::setIvec4(std::string_view name, const glm::ivec4& value) const -> void {
    glUniform4i(getLocation(name), value.x, value.y, value.z, value.w);
  }

  auto Shader::setUvec2(std::string_view name, const glm::uvec2& value) const -> void {
    glUniform2ui(getLocation(name), value.x, value.y);
  }

  auto Shader::setUvec3(std::string_view name, const glm::uvec3& value) const -> void {
    glUniform3ui(getLocation(name), value.x, value.y, value.z);
  }

  auto Shader::setUvec4(std::string_view name, const glm::uvec4& value) const -> void {
    glUniform4ui(getLocation(name), value.x, value.y, value.z, value.w);
  }

  auto Shader::setMat2(std::string_view name, const glm::mat2& value) const -> void {
    glUniformMatrix2fv(getLocation(name), 1, GL_FALSE, glm::value_ptr(value));
  }

  auto Shader::setMat3(std::string_view name, const glm::mat3& value) const -> void {
    glUniformMatrix3fv(getLocation(name), 1, GL_FALSE, glm::value_ptr(value));
  }

  auto Shader::setMat4(std::string_view name, const glm::mat4& value) const -> void {
    glUniformMatrix4fv(getLocation(name), 1, GL_FALSE, glm::value_ptr(value));
  }



  CubeShader::CubeShader() : Shader{ "cube.vs"sv, "cube.fs"sv } {}

  auto CubeShader::setModel(const glm::mat4& model) const -> void {
    setMat4("model"sv, model);
  }

  auto CubeShader::setView(const glm::mat4& view) const -> void {
    setMat4("view"sv, view);
  }

  auto CubeShader::setProjection(const glm::mat4& projection) const -> void {
    setMat4("projection"sv, projection);
  }

  auto CubeShader::setObjectColor(const glm::vec4& color) const -> void {
    setVec4("object_color"sv, color);
  }

  auto CubeShader::setLightColor(const glm::vec4& color) const -> void {
    setVec4("light_color"sv, color);
  }



  LightCubeShader::LightCubeShader() : Shader{ "light_cube.vs"sv, "light_cube.fs"sv } {}

  auto LightCubeShader::setModel(const glm::mat4& model) const -> void {
    setMat4("model"sv, model);
  }

  auto LightCubeShader::setView(const glm::mat4& view) const -> void {
    setMat4("view"sv, view);
  }

  auto LightCubeShader::setProjection(const glm::mat4& projection) const -> void {
    setMat4("projection"sv, projection);
  }

  auto LightCubeShader::setLightColor(const glm::vec4& color) const -> void {
    setVec4("light_color"sv, color);
  }

}  // namespace brabbit
