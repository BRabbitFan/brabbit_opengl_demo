#include <brabbit/camera.hpp>

namespace brabbit {

  Camera::Camera() {
    updateView();
  }

  auto Camera::getPosition() const -> const glm::vec3& {
    return position_;
  }

  auto Camera::setPosition(const glm::vec3& position) -> void {
    position_ = position;
    updateTarget();
    updateView();
  }

  auto Camera::setPosition(glm::vec3&& position) -> void {
    position_ = std::move(position);
    updateTarget();
    updateView();
  }

  auto Camera::getFront() const -> const glm::vec3& {
    return front_;
  }

  auto Camera::setFront(const glm::vec3& front) -> void {
    front_ = front;
    updateTarget();
    updateView();
  }

  auto Camera::setFront(glm::vec3&& front) -> void {
    front_ = std::move(front);
    updateTarget();
    updateView();
  }

  auto Camera::getUp() const -> const glm::vec3& {
    return up_;
  }

  auto Camera::setUp(const glm::vec3& up) -> void {
    up_ = up;
    updateView();
  }

  auto Camera::setUp(glm::vec3&& up) -> void {
    up_ = std::move(up);
    updateView();
  }

  auto Camera::getTarget() const -> const glm::vec3& {
    return target_;
  }

  auto Camera::setTarget(const glm::vec3& target) -> void {
    target_ = target;
    updateFront();
    updateView();
  }

  auto Camera::setTarget(glm::vec3&& target) -> void {
    target_ = std::move(target);
    updateFront();
    updateView();
  }

  auto Camera::getBack() const -> glm::vec3 {
    return -front_;
  }

  auto Camera::getDown() const -> glm::vec3 {
    return -up_;
  }

  auto Camera::getLeft() const -> glm::vec3 {
    return glm::normalize(glm::cross(up_, front_));
  }

  auto Camera::getRight() const -> glm::vec3 {
    return glm::normalize(glm::cross(front_, up_));
  }

  auto Camera::getYaw() const -> float {
    return glm::degrees(std::atan2(front_.z, front_.x));
  }

  auto Camera::getPitch() const -> float {
    return glm::degrees(std::asin(front_.y));
  }

  auto Camera::getView() const -> const glm::mat4& {
    return view_;
  }

  auto Camera::getFov() const -> float {
    return fov_;
  }

  auto Camera::setFov(float fov) -> void {
    fov_ = fov;
    updateProjection();
  }

  auto Camera::getWidth() const -> float {
    return width_;
  }

  auto Camera::setWidth(float width) -> void {
    width_ = width;
    updateProjection();
  }

  auto Camera::getHeight() const -> float {
    return height_;
  }

  auto Camera::setHeight(float height) -> void {
    height_ = height;
    updateProjection();
  }

  auto Camera::getSize() const -> glm::vec2 {
    return { width_, height_ };
  }

  auto Camera::setSize(const glm::vec2& size) -> void {
    width_ = size.x;
    height_ = size.y;
    updateProjection();
  }

  auto Camera::getNear() const -> float {
    return near_;
  }

  auto Camera::setNear(float near) -> void {
    near_ = near;
    updateProjection();
  }

  auto Camera::getFar() const -> float {
    return far_;
  }

  auto Camera::setFar(float far) -> void {
    far_ = far;
    updateProjection();
  }

  auto Camera::getProjection() const -> const glm::mat4& {
    return projection_;
  }

  auto Camera::getSpeed() const -> float {
    return speed_;
  }

  auto Camera::setSpeed(float speed) -> void {
    speed_ = speed;
  }

  auto Camera::updateFront() -> void {
    front_ = glm::normalize(target_ - position_);
  }

  auto Camera::updateTarget() -> void {
    target_ = position_ + front_;
  }

  auto Camera::updateView() -> void {
    view_ = glm::lookAt(position_, target_, up_);
  }

  auto Camera::updateProjection() -> void {
    projection_ = glm::perspective(glm::radians(fov_), width_ / height_, near_, far_);
  }

}  // namespace brabbit
