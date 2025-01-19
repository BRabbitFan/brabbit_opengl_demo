#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace brabbit {

  class Camera {
   public:
    explicit Camera();
    virtual ~Camera() = default;

   public:
    auto position() const -> const glm::vec3&;
    auto setPosition(const glm::vec3& position) -> void;
    auto setPosition(glm::vec3&& position) -> void;

    auto front() const -> const glm::vec3&;
    auto setFront(const glm::vec3& front) -> void;
    auto setFront(glm::vec3&& front) -> void;

    auto up() const -> const glm::vec3&;
    auto setUp(const glm::vec3& up) -> void;
    auto setUp(glm::vec3&& up) -> void;

    auto right() const -> glm::vec3;

    auto target() const -> const glm::vec3&;
    auto setTarget(const glm::vec3& target) -> void;
    auto setTarget(glm::vec3&& target) -> void;

    auto yaw() const -> float;
    auto pitch() const -> float;

    auto view() const -> const glm::mat4&;

    auto speed() const -> float;
    auto setSpeed(float speed) -> void;

   private:
    auto updateFront() -> void;
    auto updateTarget() -> void;
    auto updateView() -> void;

   private:
    glm::vec3 position_{ 0.0f, 0.0f, 3.0f };
    glm::vec3 front_{ 0.0f, 0.0f, -1.0f };
    glm::vec3 up_{ 0.0f, 1.0f, 0.0f };
    glm::vec3 target_{ 0.0f, 0.0f, 0.0f };

    glm::mat4 view_{ 1.0f };

    float speed_{ 5.0f };
  };

}  // namespace brabbit
