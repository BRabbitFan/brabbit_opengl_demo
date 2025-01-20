#pragma once

#include <map>
#include <memory>
#include <string_view>
#include <type_traits>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace brabbit {

  class Shader {
   public:
    virtual ~Shader();

   public:
    auto getId() const -> GLuint;

    auto use() -> void;

   protected:
    explicit Shader(std::string_view vertex_name, std::string_view fragment_name);

   protected:
    auto setFloat(std::string_view name, GLfloat value) const -> void;
    auto setInt(std::string_view name, GLint value) const -> void;
    auto setUint(std::string_view name, GLuint value) const -> void;

    auto setVec2(std::string_view name, const glm::vec2& value) const -> void;
    auto setVec3(std::string_view name, const glm::vec3& value) const -> void;
    auto setVec4(std::string_view name, const glm::vec4& value) const -> void;

    auto setIvec2(std::string_view name, const glm::ivec2& value) const -> void;
    auto setIvec3(std::string_view name, const glm::ivec3& value) const -> void;
    auto setIvec4(std::string_view name, const glm::ivec4& value) const -> void;

    auto setUvec2(std::string_view name, const glm::uvec2& value) const -> void;
    auto setUvec3(std::string_view name, const glm::uvec3& value) const -> void;
    auto setUvec4(std::string_view name, const glm::uvec4& value) const -> void;

    auto setMat2(std::string_view name, const glm::mat2& value) const -> void;
    auto setMat3(std::string_view name, const glm::mat3& value) const -> void;
    auto setMat4(std::string_view name, const glm::mat4& value) const -> void;

   private:
    auto getLocation(std::string_view name) const -> GLint;

   private:
    GLuint id_{ 0 };
  };

  template <typename _Type>
  auto LoadCachedShader() -> _Type* {
    static_assert(std::is_base_of_v<Shader, _Type>, "_Type must be derived from Shader");

    const char* type = typeid(_Type).name();

    static std::map<const char*, std::unique_ptr<Shader>> Cache;
    if (auto iter = Cache.find(type); iter != Cache.cend()) {
      return static_cast<_Type*>(iter->second.get());
    }

    auto base = std::make_unique<_Type>();
    auto* derived = static_cast<_Type*>(base.get());
    Cache.emplace(type, std::move(base));
    return derived;
  }

}  // namespace brabbit
