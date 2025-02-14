#pragma once

#include <memory>
#include <type_traits>
#include <vector>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <brabbit/camera.hpp>
#include <brabbit/shader.hpp>
#include <brabbit/scene_object.hpp>
#include <brabbit/light.hpp>

namespace brabbit {

  class Window;
  class SceneObject;
  class Light;

  class Scene {
    friend class Window;

   public:
    explicit Scene();
    explicit Scene(std::size_t width, std::size_t height, std::size_t depth);
    virtual ~Scene();

   public:
    auto getWindow() const -> const Window*;
    auto getWindow() -> Window*;
    auto setWindow(Window* window) -> void;

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

    auto drawObjects() -> void;

   public:
    auto getCamera() const -> const Camera*;
    auto getCamera() -> Camera*;

    auto getLight() const -> const Light*;
    auto getLight() -> Light*;

   protected:
    auto processFrame(double delta_time) -> void;

   private:
    auto updateScaleFactor() -> void;

   private:
    Window* window_{ nullptr };

    std::size_t width_{ 1 };
    std::size_t height_{ 1 };
    std::size_t depth_{ 1 };

    double scale_factor_{ 1.0 };

    std::unique_ptr<Camera> camera_{ nullptr };

    std::vector<std::unique_ptr<SceneObject>> objects_{};
    Light* light_{ nullptr };
  };

}  // namespace brabbit
