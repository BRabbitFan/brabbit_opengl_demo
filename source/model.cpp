#include <array>
#include <filesystem>
#include <fstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <float.h>
#include <model.hpp>

namespace brabbit {

  using namespace std::string_literals;
  using namespace std::string_view_literals;

  namespace {

    struct Stl {
      struct Facet {
        glm::vec3 normal{};
        std::array<glm::vec3, 3> vertices{};
      };

      std::string name{};
      std::vector<Facet> facets{};
    };

    auto Trim(std::string& string) -> void {
      constexpr auto notspace = [](unsigned char ch) { return !std::isspace(ch); };
      string.erase(string.begin(), std::find_if(string.begin(), string.end(), notspace));
      string.erase(std::find_if(string.rbegin(), string.rend(), notspace).base(), string.end());
    }

    auto LoadStlFile(std::string_view name) -> Stl {
      auto path = std::filesystem::current_path() / "resource"sv / "model"sv / name;
      if (!std::filesystem::exists(path)) {
        return {};
      }

      auto file = std::ifstream{ path, std::ios::binary };
      if (!file) {
        return {};
      }

      auto stl = Stl{};
      auto line = std::string{};
      auto index = -1;
      while (std::getline(file, line)) {
        Trim(line);

        if (line.empty()) {
          continue;
        }

        constexpr auto BEGIN_SOLID = "solid "sv;
        constexpr auto BEGIN_FACET = "facet normal "sv;
        constexpr auto BEGIN_LOOP  = "outer loop"sv;
        constexpr auto VERTEX      = "vertex "sv;
        constexpr auto END_LOOP    = "endloop"sv;
        constexpr auto END_FACET   = "endfacet"sv;
        constexpr auto END_SOLID   = "endsolid"sv;

        if (line.starts_with(BEGIN_SOLID)) {
          stl.name = line.substr(BEGIN_SOLID.size());
          continue;
        }

        if (line.starts_with(BEGIN_FACET)) {
          auto& facet = stl.facets.emplace_back();
          auto stream = std::istringstream{ line.substr(BEGIN_FACET.size()) };
          stream >> facet.normal.x >> facet.normal.y >> facet.normal.z;
          if (stream.fail()) {
            stl.facets.pop_back();
          }

          continue;
        }

        if (line.starts_with(BEGIN_LOOP)) {
          index = 0;
          continue;
        }

        if (line.starts_with(VERTEX)) {
          if (0 > index || index > 2) {
            continue;
          }

          auto& facet = stl.facets.back();
          auto& vertex = facet.vertices.at(index);
          auto stream = std::istringstream{ line.substr(VERTEX.size()) };
          stream >> vertex.x >> vertex.y >> vertex.z;

          // cube test
          vertex.x = vertex.x / 20.0f / 2;
          vertex.y = vertex.y / 20.0f / 2;
          vertex.z = vertex.z / 20.0f / 2;

          if (stream.fail()) {
            stl.facets.pop_back();
          }

          ++index;
          continue;
        }

        if (line.starts_with(END_LOOP)) {
          if (index != 3) {
            stl.facets.pop_back();
          }

          index = -1;
          continue;
        }

        if (line.starts_with(END_FACET)) {
          continue;
        }

        if (line.starts_with(END_SOLID)) {
          continue;
        }
      }

      return stl;
    }

  }  // namespace

  Model::Model(std::string_view model_name) {
    auto stl = LoadStlFile(model_name);
    if (stl.facets.empty()) {
      return;
    }

    for (auto& facet : stl.facets) {
      indices_.emplace_back(vertices_.size(), vertices_.size() + 1, vertices_.size() + 2);
      vertices_.insert(vertices_.end(),
                       std::make_move_iterator(facet.vertices.begin()),
                       std::make_move_iterator(facet.vertices.end()));
    }
  }

  auto Model::vertices() const -> const std::vector<glm::vec3>& {
    return vertices_;
  }

  auto Model::verticesData() const -> const float* {
    return glm::value_ptr(vertices_.front());
  }

  auto Model::verticesSize() const -> std::size_t {
    return vertices_.size() * sizeof(glm::vec3);
  }

  auto Model::indices() const -> const std::vector<glm::uvec3>& {
    return indices_;
  }

  auto Model::indicesData() const -> const glm::uint* {
    return glm::value_ptr(indices_.front());
  }

  auto Model::indicesSize() const -> std::size_t {
    return indices_.size() * sizeof(glm::uvec3);
  }

}  // namespace brabbit
