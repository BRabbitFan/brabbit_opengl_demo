#pragma once

#include <memory>
#include <type_traits>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace brabbit {

  class SceneObject;
  class Scene {
   public:
    explicit Scene();
    explicit Scene(std::size_t width, std::size_t height, std::size_t depth);
    virtual ~Scene();

   public:
    auto getWidth() const -> std::size_t;
    auto setWidth(std::size_t width) -> void;

    auto getHeight() const -> std::size_t;
    auto setHeight(std::size_t height) -> void;

    auto getDepth() const -> std::size_t;
    auto setDepth(std::size_t depth) -> void;

    auto getScaleFactor() const -> double;

   public:
    auto objects() const -> const std::vector<std::unique_ptr<SceneObject>>&;
    auto containsObject(const SceneObject* object) const -> bool;
    auto emplaceObject(std::unique_ptr<SceneObject>&& object) -> SceneObject*;

    template <typename _Type, typename... _Args>
    auto emplaceObject(_Args&&... args) -> _Type* {
      static_assert(std::is_base_of_v<SceneObject, _Type>,
                    "_Type must be derived from SceneObject");
      auto* base = emplaceObject(std::make_unique<_Type>(std::forward<_Args>(args)...));
      return static_cast<_Type*>(base);
    }

    auto eraseObject(const SceneObject* object) -> void;
    auto takeObject(const SceneObject* object) -> std::unique_ptr<SceneObject>;

   private:
    auto updateScaleFactor() -> void;

   private:
    std::size_t width_{ 1 };
    std::size_t height_{ 1 };
    std::size_t depth_{ 1 };

    double scale_factor_{ 1.0 };

    std::vector<std::unique_ptr<SceneObject>> objects_{};
  };



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

    auto getScene() const -> const Scene*;
    auto getScene() -> Scene*;

   public:
    virtual auto draw() -> void = 0;

   protected:
    glm::mat4 model_{ 1.0f };
    Scene* scene_{ nullptr };
    unsigned int vao_{ 0 };
    unsigned int vbo_{ 0 };
    unsigned int ebo_{ 0 };
  };

}  // namespace brabbit
