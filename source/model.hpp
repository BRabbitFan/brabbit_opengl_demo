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

   protected:
    std::vector<glm::vec3> vertices_{};
    std::vector<glm::uvec3> indices_{};
  };

}  // namespace brabbit
