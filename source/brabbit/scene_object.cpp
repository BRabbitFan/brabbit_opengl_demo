#include <brabbit/scene_object.hpp>
#include <brabbit/scene.hpp>

namespace brabbit {

  SceneObject::SceneObject() = default;
  SceneObject::~SceneObject() = default;

  auto SceneObject::getModel() const -> const glm::mat4& {
    return model_;
  }

  auto SceneObject::setModel(const glm::mat4& model) -> void {
    model_ = model;
  }

  auto SceneObject::setModel(glm::mat4&& model) -> void {
    model_ = std::move(model);
  }

  auto SceneObject::getScaledModel() const -> glm::mat4 {
    auto factor = scene_->getScaleFactor();
    return glm::scale(model_, { factor, factor, factor });
  }

  auto SceneObject::getShader() const -> const Shader* {
    return shader_;
  }

  auto SceneObject::getShader() -> Shader* {
    return shader_;
  }

  auto SceneObject::setShader(Shader* shader) -> void {
    shader_ = shader;
  }

  auto SceneObject::getScene() const -> const Scene* {
    return scene_;
  }

  auto SceneObject::getScene() -> Scene* {
    return scene_;
  }

  auto SceneObject::draw() -> void {}

}  // namespace brabbit
