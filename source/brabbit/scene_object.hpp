#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <brabbit/shader.hpp>

namespace brabbit {

  class Scene;
  class SceneObject {
    friend class Scene;

   public:
    explicit SceneObject();
    virtual ~SceneObject();

   public:
    auto getModel() const -> const glm::mat4&;
    auto setModel(const glm::mat4& model) -> void;
    auto setModel(glm::mat4&& model) -> void;

    auto getScaledModel() const -> glm::mat4;

    auto getShader() const -> const Shader*;
    auto getShader() -> Shader*;
    auto setShader(Shader* shader) -> void;

    auto getScene() const -> const Scene*;
    auto getScene() -> Scene*;

   protected:
    virtual auto draw() -> void;

   protected:
    glm::mat4 model_{ 1.0f };

    Shader* shader_{ nullptr };
    Scene* scene_{ nullptr };

    unsigned int vao_{ 0 };
  };

}  // namespace brabbit
