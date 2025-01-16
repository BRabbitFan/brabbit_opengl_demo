#include <filesystem>
#include <fstream>
#include <string>
#include <string_view>

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

  Shader::Shader(std::string_view vertex, std::string_view fragment) {
    id_ = glCreateProgram();

    auto vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    auto vertex_shader_source = LoadShaderSource(vertex);
    auto vertex_shader_source_cstr = vertex_shader_source.c_str();
    glShaderSource(vertex_shader, 1, &vertex_shader_source_cstr, NULL);
    glCompileShader(vertex_shader);
    glAttachShader(id_, vertex_shader);

    auto fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    auto fragment_shader_source = LoadShaderSource(fragment);
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

  auto Shader::setVec2(std::string_view name, GLvec2 value) const -> void {
    auto& [v0, v1] = value;
    glUniform2f(getLocation(name), v0, v1);
  }

  auto Shader::setVec3(std::string_view name, GLvec3 value) const -> void {
    auto& [v0, v1, v2] = value;
    glUniform3f(getLocation(name), v0, v1, v2);
  }

  auto Shader::setVec4(std::string_view name, GLvec4 value) const -> void {
    auto& [v0, v1, v2, v3] = value;
    glUniform4f(getLocation(name), v0, v1, v2, v3);
  }

  auto Shader::setInt(std::string_view name, GLint value) const -> void {
    glUniform1i(getLocation(name), value);
  }

  auto Shader::setIvec2(std::string_view name, GLivec2 value) const -> void {
    auto& [v0, v1] = value;
    glUniform2i(getLocation(name), v0, v1);
  }

  auto Shader::setIvec3(std::string_view name, GLivec3 value) const -> void {
    auto& [v0, v1, v2] = value;
    glUniform3i(getLocation(name), v0, v1, v2);
  }

  auto Shader::setIvec4(std::string_view name, GLivec4 value) const -> void {
    auto& [v0, v1, v2, v3] = value;
    glUniform4i(getLocation(name), v0, v1, v2, v3);
  }

  auto Shader::setUint(std::string_view name, GLuint value) const -> void {
    glUniform1ui(getLocation(name), value);
  }

  auto Shader::setUvec2(std::string_view name, GLuvec2 value) const -> void {
    auto& [v0, v1] = value;
    glUniform2ui(getLocation(name), v0, v1);
  }

  auto Shader::setUvec3(std::string_view name, GLuvec3 value) const -> void {
    auto& [v0, v1, v2] = value;
    glUniform3ui(getLocation(name), v0, v1, v2);
  }

  auto Shader::setUvec4(std::string_view name, GLuvec4 value) const -> void {
    auto& [v0, v1, v2, v3] = value;
    glUniform4ui(getLocation(name), v0, v1, v2, v3);
  }



  CubeShader::CubeShader() : Shader{ "cube.vs"sv, "cube.fs"sv } {}

  auto CubeShader::setGlobalColor(GLvec4 color) const -> void {
    setVec4("global_color"sv, color);
  }

}  // namespace brabbit
