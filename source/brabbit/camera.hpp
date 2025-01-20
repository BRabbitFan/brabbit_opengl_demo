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
    auto getPosition() const -> const glm::vec3&;
    auto setPosition(const glm::vec3& position) -> void;
    auto setPosition(glm::vec3&& position) -> void;

    auto getFront() const -> const glm::vec3&;
    auto setFront(const glm::vec3& front) -> void;
    auto setFront(glm::vec3&& front) -> void;

    auto getUp() const -> const glm::vec3&;
    auto setUp(const glm::vec3& up) -> void;
    auto setUp(glm::vec3&& up) -> void;

    auto getTarget() const -> const glm::vec3&;
    auto setTarget(const glm::vec3& target) -> void;
    auto setTarget(glm::vec3&& target) -> void;

    auto getBack() const -> glm::vec3;
    auto getDown() const -> glm::vec3;
    auto getLeft() const -> glm::vec3;
    auto getRight() const -> glm::vec3;

    auto getYaw() const -> float;
    auto getPitch() const -> float;

    auto getView() const -> const glm::mat4&;

   public:
    auto getFov() const -> float;
    auto setFov(float fov) -> void;

    auto getWidth() const -> float;
    auto setWidth(float width) -> void;

    auto getHeight() const -> float;
    auto setHeight(float height) -> void;

    auto getSize() const -> glm::vec2;
    auto setSize(const glm::vec2& size) -> void;

    auto getNear() const -> float;
    auto setNear(float near) -> void;

    auto getFar() const -> float;
    auto setFar(float far) -> void;

    auto getProjection() const -> const glm::mat4&;

   public:
    auto getSpeed() const -> float;
    auto setSpeed(float speed) -> void;

   private:
    auto updateFront() -> void;
    auto updateTarget() -> void;
    auto updateView() -> void;
    auto updateProjection() -> void;

   private:
    glm::vec3 position_{ 0.0f, 0.0f, 3.0f };
    glm::vec3 front_{ 0.0f, 0.0f, -1.0f };
    glm::vec3 up_{ 0.0f, 1.0f, 0.0f };
    glm::vec3 target_{ 0.0f, 0.0f, 0.0f };

    float fov_{ 45.0f };
    float width_{ 800.0f };
    float height_{ 600.0f };
    float near_{ 0.1f };
    float far_{ 100.0f };
    glm::mat4 view_{ 1.0f };
    glm::mat4 projection_{ 1.0f };

    float speed_{ 5.0f };
  };

}  // namespace brabbit
