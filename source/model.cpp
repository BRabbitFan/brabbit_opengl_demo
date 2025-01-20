#include <array>
#include <filesystem>
#include <fstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <float.h>
#include <model.hpp>
#include "shader.hpp"

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
      normals_.insert(normals_.end(), 3, facet.normal);
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

  auto Model::normals() const -> const std::vector<glm::vec3>& {
    return normals_;
  }

  auto Model::normalsData() const -> const float* {
    return glm::value_ptr(normals_.front());
  }

  auto Model::normalsSize() const -> std::size_t {
    return normals_.size() * sizeof(glm::vec3);
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

  ModelObject::ModelObject(std::unique_ptr<Model>& data) : ModelObject{ data.get() } {}

  ModelObject::ModelObject(Model* data) : data_{ data } {
    shader_ = LoadCachedShader<CubeShader>();



    // Create and bind a VAO(Vertex Array Object)
    glGenVertexArrays(1, &vao_);



    // Load attributes in VAO
    // From now on, any function call in this target will action on our VAO buffer.
    glBindVertexArray(vao_);

    // Generate a VBO(Vertex Buffer Object) buffer.
    // This buffer is use to send Vertex data to GPU from CPU.
    glGenBuffers(1, &vbo_);

    // Bind VBO buffer to GL_ARRAY_BUFFER(array buffer) target.
    // From now on, any function call in this target will action on our VBO buffer.
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);

    // Copy real vertices data into VBO buffer.
    // param 4:
    // GL_STATIC_DRAW: The data will never or rarely change.
    // GL_DYNAMIC_DRAW：The data will be changed a lot.
    // GL_STREAM_DRAW：The data changes every time it is plotted.
    glBufferData(GL_ARRAY_BUFFER, data->verticesSize(), data->verticesData(), GL_STATIC_DRAW);

    // EBO/IBO (Element Buffer Object/Index Buffer Object)
    glGenBuffers(1, &ebo_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data->indicesSize(), data->indicesData(), GL_STATIC_DRAW);

    // Tell GPU how to decode our vertices data.
    // Set attribute pointer's infomation about our vertices data. (save in VAO)
    // param 1: [int] location index in vertex shader source: 'layout(location = 0)'
    // param 2: [int] size of data, 3 means it's a 'vec3' attribute
    // param 3: [enum] type of data, GL_FLOAT means float (of course..)
    // param 4: [bool] need to normalize or not, GL_TRUE or GL_FALSE
    // param 5: [int] stride, data's stride between each group
    // param 6: [void*] offset, offset of data's begin position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<void*>(0));
    // Make data attribute in location 0 enabled
    // Call the 'glDisableVertexAttribArray' in some where to disabled it.
    glEnableVertexAttribArray(0);



    glGenBuffers(1, &normal_vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, normal_vbo_);
    glBufferData(GL_ARRAY_BUFFER, data->normalsSize(), data->normalsData(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(1);
  }

  ModelObject::~ModelObject() {

  }

  auto ModelObject::data() const -> const Model* {
    return data_;
  }

  auto ModelObject::setData(Model* data) -> void {
    data_ = data;
  }

  auto ModelObject::draw(const glm::mat4& view, const glm::mat4& projection) -> void {
    auto* shader = static_cast<CubeShader*>(shader_);

    shader->use();

    auto current_frame_time = glfwGetTime();
    GLfloat r = std::sin(current_frame_time) / 2.0f + 0.3f;
    GLfloat g = std::cos(current_frame_time) / 2.0f + 0.4f;
    GLfloat b = std::sin(current_frame_time) / 2.0f + 0.5f;
    shader->setObjectColor({ r, g, b, 1.0f });
    auto* light_cube = scene_->getLightCube();
    shader->setLightColor(light_cube->getColor());
    shader->setLightPosition(light_cube->getPosition());

    auto radians = static_cast<float>(current_frame_time) * glm::radians(50.0f);
    model_ = glm::rotate(glm::mat4{ 1.0f }, radians, { 0.5f, 1.0f, 0.0f });
    shader->setModel(getScaledModel());
    shader->setView(view);
    shader->setProjection(projection);

    // Load attributes in VAO
    glBindVertexArray(vao_);

    // Use EBO and VB0 to draw the triangle
    // param 1: [enum] type to draw
    // param 2: [int] index count of element array
    // param 3: [enum] type of index array
    // param 4: [void*] offset of index array
    glDrawElements(GL_TRIANGLES, data_->indicesSize(), GL_UNSIGNED_INT, reinterpret_cast<void*>(0));
  }

}  // namespace brabbit
