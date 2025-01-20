#pragma once

#include <memory>
#include <string>
#include <string_view>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <brabbit/scene.hpp>

namespace brabbit {

  class Window {
   public:
    explicit Window(std::string_view title);
    explicit Window(std::size_t width, std::size_t height);
    explicit Window(std::string_view title, std::size_t width, std::size_t height);
    virtual ~Window();

   public:
    auto isVaild() const -> bool;
    auto getHandle() const -> GLFWwindow*;

    auto getTitle() const -> std::string;
    auto setTitle(std::string_view title) -> void;

    auto getWidth() const -> std::size_t;
    auto setWidth(std::size_t width) -> void;

    auto getHeight() const -> std::size_t;
    auto setHeight(std::size_t height) -> void;

    auto getSize() const -> glm::vec2;
    auto setSize(const glm::vec2& size) -> void;

    auto getScene() const -> const Scene*;
    auto getScene() -> Scene*;
    auto setScene(std::unique_ptr<Scene>&& scene) -> Scene*;

    template <typename _Type, typename... _Args>
    auto setScene(_Args&&... args) -> _Type* {
      static_assert(std::is_base_of_v<Scene, _Type>, "_Type must be derived from Scene");
      setScene(std::make_unique<_Type>(std::forward<_Args>(args)...));
      return static_cast<_Type*>(scene_.get());
    }

   public:
    auto executeLoop() -> int;

   private:
    GLFWwindow* handle_{ nullptr };
    std::string title_{};
    std::size_t width_{ 800 };
    std::size_t height_{ 600 };
    std::unique_ptr<Scene> scene_{ nullptr };
  };

}  // namespace brabbit
