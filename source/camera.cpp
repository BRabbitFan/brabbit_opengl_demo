#include "camera.hpp"
#include <camera.hpp>

namespace brabbit {

  Camera::Camera() {
    updateView();
  }

  auto Camera::position() const -> const glm::vec3& {
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

  auto Camera::front() const -> const glm::vec3& {
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

  auto Camera::up() const -> const glm::vec3& {
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

  auto Camera::right() const -> glm::vec3 {
    return glm::normalize(glm::cross(front_, up_));
  }

  auto Camera::target() const -> const glm::vec3& {
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

  auto Camera::yaw() const -> float {
    return glm::degrees(std::atan2(front_.z, front_.x));
  }

  auto Camera::pitch() const -> float {
    return glm::degrees(std::asin(front_.y));
  }

  auto Camera::view() const -> const glm::mat4& {
    return view_;
  }

  auto Camera::speed() const -> float {
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

}  // namespace brabbit
