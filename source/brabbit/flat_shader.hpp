#pragma once

#include <brabbit/shader.hpp>

namespace brabbit {

  class FlatShader : public Shader {
   public:
    explicit FlatShader();
    virtual ~FlatShader() override;

   public:
    auto setModel(const glm::mat4& model) const -> void;
    auto setView(const glm::mat4& view) const -> void;
    auto setProjection(const glm::mat4& projection) const -> void;

    auto setLightColor(const glm::vec4& color) const -> void;
  };

}  // namespace brabbit
