#pragma once

#include <string_view>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace brabbit {

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
    explicit Shader(std::string_view vertex_name, std::string_view fragment_name);

   protected:
    auto setFloat(std::string_view name, GLfloat value) const -> void;
    auto setInt(std::string_view name, GLint value) const -> void;
    auto setUint(std::string_view name, GLuint value) const -> void;

    auto setVec2(std::string_view name, glm::vec2 value) const -> void;
    auto setVec3(std::string_view name, glm::vec3 value) const -> void;
    auto setVec4(std::string_view name, glm::vec4 value) const -> void;

    auto setIvec2(std::string_view name, glm::ivec2 value) const -> void;
    auto setIvec3(std::string_view name, glm::ivec3 value) const -> void;
    auto setIvec4(std::string_view name, glm::ivec4 value) const -> void;

    auto setUvec2(std::string_view name, glm::uvec2 value) const -> void;
    auto setUvec3(std::string_view name, glm::uvec3 value) const -> void;
    auto setUvec4(std::string_view name, glm::uvec4 value) const -> void;

    auto setMat2(std::string_view name, glm::mat2 value) const -> void;
    auto setMat3(std::string_view name, glm::mat3 value) const -> void;
    auto setMat4(std::string_view name, glm::mat4 value) const -> void;

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
    auto setGlobalColor(glm::vec4 color) const -> void;
    auto setTransform(glm::mat4 transform) const -> void;
  };

}  // namespace brabbit
