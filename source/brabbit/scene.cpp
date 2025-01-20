#include <algorithm>
#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <brabbit/camera.hpp>
#include <brabbit/scene.hpp>
#include <brabbit/window.hpp>

namespace brabbit {

  namespace {

    template <typename _Type>
    auto GenerateContainsChecker(const _Type* object) {
      return [object](const std::unique_ptr<_Type>& obj) { return obj.get() == object; };
    }

  }  // namespace

  Scene::Scene() : Scene{ 1, 1, 1 } {}

  Scene::Scene(std::size_t width, std::size_t height, std::size_t depth)
      : width_{ width }, height_{ height }, depth_{ depth } , camera_{std::make_unique<Camera>()} {
    updateScaleFactor();
    light_ = emplaceObject<Light>();
  }

  Scene::~Scene() {}

  auto Scene::getWindow() const -> const Window* {
    return window_;
  }

  auto Scene::getWindow() -> Window* {
    return window_;
  }

  auto Scene::setWindow(Window* window) -> void {
    window_ = window;
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

  auto Scene::drawObjects() -> void {
    for (const auto& object : objects_) {
      if (object) {
        object->draw();
      }
    }
  }

  auto Scene::getCamera() const -> const Camera* {
    return camera_.get();
  }

  auto Scene::getCamera() -> Camera* {
    return camera_.get();
  }

  auto Scene::getLight() const -> const Light* {
    return light_;
  }

  auto Scene::getLight() -> Light* {
    return light_;
  }

  auto Scene::processFrame(double delta_time) -> void {
    auto* handle = window_->getHandle();
    if (!handle) {
      return;
    }

    if (glfwGetKey(handle, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(handle, true);
    }

    auto position = camera_->getPosition();
    const auto speed = camera_->getSpeed() * static_cast<float>(delta_time);

    if (glfwGetKey(handle, GLFW_KEY_W) == GLFW_PRESS) {
      position += camera_->getFront() * speed;
    }

    if (glfwGetKey(handle, GLFW_KEY_S) == GLFW_PRESS) {
      position += camera_->getBack() * speed;
    }

    if (glfwGetKey(handle, GLFW_KEY_A) == GLFW_PRESS) {
      position += camera_->getLeft() * speed;
    }

    if (glfwGetKey(handle, GLFW_KEY_D) == GLFW_PRESS) {
      position += camera_->getRight() * speed;
    }

    if (glfwGetKey(handle, GLFW_KEY_SPACE) == GLFW_PRESS) {
      position += camera_->getUp() * speed;
    }

    if (glfwGetKey(handle, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
      position += camera_->getDown() * speed;
    }

    camera_->setPosition(std::move(position));

    static auto last_x = -1.0;
    static auto last_y = -1.0;
    auto cursor_x = 0.0;
    auto cursor_y = 0.0;
    glfwGetCursorPos(handle, &cursor_x, &cursor_y);

    if (last_x < 0.0 || last_y < 0.0) {
      last_x = cursor_x;
      last_y = cursor_y;
    }

    auto x_offset = cursor_x - last_x;
    auto y_offset = last_y - cursor_y;  // reversed since y-coordinates go from bottom to top
    if (x_offset || y_offset) {
      last_x = cursor_x;
      last_y = cursor_y;

      auto sensitivity = 0.1f;
      x_offset *= sensitivity;
      y_offset *= sensitivity;

      auto yaw = camera_->getYaw() + x_offset;
      auto pitch = static_cast<float>(camera_->getPitch() + y_offset);
      pitch = std::max(-89.0f, std::min(89.0f, pitch));

      auto front = glm::vec3{
        std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch)),
        std::sin(glm::radians(pitch)),
        std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch)),
      };

      camera_->setFront(glm::normalize(front));
    }

    drawObjects();
  }

  auto Scene::updateScaleFactor() -> void {
    scale_factor_ = 1.0 / static_cast<double>(std::max({ width_, height_, depth_ }));
  }

}  // namespace brabbit
