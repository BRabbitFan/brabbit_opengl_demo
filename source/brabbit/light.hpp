#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <brabbit/scene_object.hpp>

namespace brabbit {

  class Light : public SceneObject {
   public:
    explicit Light();
    virtual ~Light() override;

   public:
    auto isLampVisible() const -> bool;
    auto setLampVisible(bool visible) -> void;

    auto getColor() const -> const glm::vec4&;
    auto setColor(const glm::vec4& color) -> void;

    auto getPosition() const -> const glm::vec3&;
    auto setPosition(const glm::vec3& position) -> void;

    auto getAmbientStrength() const -> float;
    auto setAmbientStrength(float ambient_strength) -> void;

    auto getSpecularStrength() const -> float;
    auto setSpecularStrength(float specular_strength) -> void;

   protected:
    auto draw() -> void override;

   private:
    auto updateModel() -> void;

   private:
    unsigned int vbo_{ 0 };
    unsigned int ebo_{ 0 };
    bool lamp_visible_{ false };
    glm::vec4 color_{ 1.0f, 1.0f, 1.0f, 1.0f };
    glm::vec3 position_{ 1.0f, 1.0f, 1.0f };
    float ambient_strength_{ 0.1f };
    float specular_strength_{ 0.5f };
  };

}  // namespace brabbit
