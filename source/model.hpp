#pragma once

#include <string_view>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace brabbit {

  class Model {
    Model(const Model&) = delete;
    Model(Model&&) = delete;
    auto operator=(const Model&) -> Model& = delete;
    auto operator=(Model&&) -> Model& = delete;

   public:
    explicit Model(std::string_view model_name);
    virtual ~Model() = default;

   public:
    auto vertices() const -> const std::vector<glm::vec3>&;
    auto verticesData() const -> const float*;
    auto verticesSize() const -> std::size_t;

    auto indices() const -> const std::vector<glm::uvec3>&;
    auto indicesData() const -> const glm::uint*;
    auto indicesSize() const -> std::size_t;

   public:
    auto model() const -> const glm::mat4&;
    auto getModel() const -> glm::mat4;
    auto setModel(const glm::mat4& model) -> void;
    auto setModel(glm::mat4&& model) -> void;

   protected:
    std::vector<glm::vec3> vertices_{};
    std::vector<glm::uvec3> indices_{};
    glm::mat4 model_{ 1.0f };
  };

  class Screen {
   public:
    explicit Screen(int width, int height) : width_{ width }, height_{ height } {
      update();
    }
    virtual ~Screen() = default;

   public:
    auto width() const -> int { return width_; }
    auto height() const -> int { return height_; }

    auto setSize(int width, int height) -> void {
      width_ = width;
      height_ = height;
      update();
    }

    auto projection() const -> const glm::mat4& { return projection_; }

   private:
    auto update() -> void {
      projection_ = glm::perspective(glm::radians(45.0f),
                                     static_cast<float>(width_) / static_cast<float>(height_),
                                     0.1f,
                                     100.0f);
    }

   private:
    int width_{ 1 };
    int height_{ 1 };
    glm::mat4 projection_{};
  };

}  // namespace brabbit
