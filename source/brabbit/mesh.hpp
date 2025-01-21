#pragma once

#include <string_view>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <brabbit/scene.hpp>

namespace brabbit {

  class Mesh {
   public:
    explicit Mesh(std::string_view model_name);
    virtual ~Mesh() = default;

   public:
    auto getVertices() const -> const std::vector<glm::vec3>&;
    auto getVerticesData() const -> const float*;
    auto getVerticesSize() const -> std::size_t;

    auto getNormals() const -> const std::vector<glm::vec3>&;
    auto getNormalsData() const -> const float*;
    auto getNormalsSize() const -> std::size_t;

    auto getIndices() const -> const std::vector<glm::uvec3>&;
    auto getIndicesData() const -> const glm::uint*;
    auto getIndicesSize() const -> std::size_t;

   protected:
    std::vector<glm::vec3> vertices_{};
    std::vector<glm::vec3> normals_{};
    std::vector<glm::uvec3> indices_{};
  };

}  // namespace brabbit
