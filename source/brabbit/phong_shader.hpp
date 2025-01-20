#pragma once

#include <brabbit/shader.hpp>

namespace brabbit {

  class PhongShader : public Shader {
   public:
    explicit PhongShader();
    virtual ~PhongShader() override;

   public:
    auto setModel(const glm::mat4& model) const -> void;
    auto setView(const glm::mat4& view) const -> void;
    auto setProjection(const glm::mat4& projection) const -> void;

    auto setAmbientStrength(float ambient_strength) const -> void;
    auto setSpecularStrength(float specular_strength) const -> void;

    auto setObjectColor(const glm::vec4& color) const -> void;
    auto setLightColor(const glm::vec4& color) const -> void;
    auto setLightPosition(const glm::vec3& position) const -> void;
    auto setCameraPosition(const glm::vec3& position) const -> void;
  };

}  // namespace brabbit
