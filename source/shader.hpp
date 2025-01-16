#pragma once

#include <string_view>
#include <tuple>

#include <glad/glad.h>

namespace brabbit {

  using GLvec2 = std::tuple<GLfloat, GLfloat>;
  using GLvec3 = std::tuple<GLfloat, GLfloat, GLfloat>;
  using GLvec4 = std::tuple<GLfloat, GLfloat, GLfloat, GLfloat>;

  using GLivec2 = std::tuple<GLint, GLint>;
  using GLivec3 = std::tuple<GLint, GLint, GLint>;
  using GLivec4 = std::tuple<GLint, GLint, GLint, GLint>;

  using GLuvec2 = std::tuple<GLuint, GLuint>;
  using GLuvec3 = std::tuple<GLuint, GLuint, GLuint>;
  using GLuvec4 = std::tuple<GLuint, GLuint, GLuint, GLuint>;



  class Shader {
    Shader(const Shader&) = delete;
    Shader(Shader&&) = delete;
    auto operator=(const Shader&) -> Shader& = delete;
    auto operator=(Shader&&) -> Shader& = delete;

   public:
    virtual ~Shader() = default;

   public:
    auto getId() const -> GLuint;

    auto use() -> void;

   protected:
    explicit Shader(std::string_view vertex, std::string_view fragment);

   protected:
    auto setFloat(std::string_view name, GLfloat value) const -> void;
    auto setVec2(std::string_view name, GLvec2 value) const -> void;
    auto setVec3(std::string_view name, GLvec3 value) const -> void;
    auto setVec4(std::string_view name, GLvec4 value) const -> void;
    auto setInt(std::string_view name, GLint value) const -> void;
    auto setIvec2(std::string_view name, GLivec2 value) const -> void;
    auto setIvec3(std::string_view name, GLivec3 value) const -> void;
    auto setIvec4(std::string_view name, GLivec4 value) const -> void;
    auto setUint(std::string_view name, GLuint value) const -> void;
    auto setUvec2(std::string_view name, GLuvec2 value) const -> void;
    auto setUvec3(std::string_view name, GLuvec3 value) const -> void;
    auto setUvec4(std::string_view name, GLuvec4 value) const -> void;

   private:
    auto getLocation(std::string_view name) const -> GLint;

   private:
    GLuint id_{ 0 };
  };


  class CubeShader : public Shader {
   public:
    explicit CubeShader();
    virtual ~CubeShader() override = default;

   public:
    auto setGlobalColor(GLvec4 color) const -> void;
  };

}  // namespace brabbit
