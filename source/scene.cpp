#include <algorithm>

#include <scene.hpp>

namespace brabbit {

  namespace {

    template <typename _Type>
    auto GenerateContainsChecker(const _Type* object) {
      return [object](const std::unique_ptr<_Type>& obj) { return obj.get() == object; };
    }

  }  // namespace

  Scene::Scene() {

  }

  Scene::Scene(std::size_t width, std::size_t height, std::size_t depth)
      : width_(width), height_(height), depth_(depth) {
    updateScaleFactor();
  }

  Scene::~Scene() {

  }

  auto Scene::getWidth() const -> std::size_t {
    return width_;
  }

  auto Scene::setWidth(std::size_t width) -> void {
    width_ = width;
    updateScaleFactor();
  }

  auto Scene::getHeight() const -> std::size_t {
    return height_;
  }

  auto Scene::setHeight(std::size_t height) -> void {
    height_ = height;
    updateScaleFactor();
  }

  auto Scene::getDepth() const -> std::size_t {
    return depth_;
  }

  auto Scene::setDepth(std::size_t depth) -> void {
    depth_ = depth;
    updateScaleFactor();
  }

  auto Scene::getScaleFactor() const -> double {
    return scale_factor_;
  }

  auto Scene::objects() const -> const std::vector<std::unique_ptr<SceneObject>>& {
    return objects_;
  }

  auto Scene::containsObject(const SceneObject* object) const -> bool {
    return std::any_of(objects_.cbegin(), objects_.cend(), GenerateContainsChecker(object));
  }

  auto Scene::emplaceObject(std::unique_ptr<SceneObject>&& object) -> SceneObject* {
    if (!object) {
      return nullptr;
    }

    object->scene_ = this;
    return objects_.emplace_back(std::move(object)).get();
  }

  auto Scene::eraseObject(const SceneObject* object) -> void {
    objects_.erase(
        std::remove_if(objects_.begin(), objects_.end(), GenerateContainsChecker(object)),
        objects_.end());
  }

  auto Scene::takeObject(const SceneObject* object) -> std::unique_ptr<SceneObject> {
    auto iter = std::find_if(objects_.begin(), objects_.end(), GenerateContainsChecker(object));
    if (iter == objects_.end()) {
      return nullptr;
    }

    auto obj = std::move(*iter);
    objects_.erase(iter);
    return obj;
  }

  auto Scene::updateScaleFactor() -> void {
    scale_factor_ = 1.0 / static_cast<double>(std::max({ width_, height_, depth_ }));
  }



  SceneObject::SceneObject() {

  }

  SceneObject::~SceneObject() {

  }

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

  auto SceneObject::getScene() const -> const Scene* {
    return scene_;
  }

  auto SceneObject::getScene() -> Scene* {
    return scene_;
  }

}  // namespace brabbit
