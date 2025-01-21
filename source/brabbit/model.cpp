#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <brabbit/model.hpp>
#include <brabbit/phong_shader.hpp>

namespace brabbit {

  Model::Model(std::unique_ptr<Mesh>& mesh) : Model{ mesh.get() } {}

  Model::Model(Mesh* mesh) : mesh_{ mesh } {
    if (!mesh_) {
      return;
    }

    shader_ = LoadCachedShader<PhongShader>();



    // Create and bind a VAO(Vertex Array Object)
    glGenVertexArrays(1, &vao_);

    // Load attributes in VAO
    // From now on, any function call in this target will action on our VAO buffer.
    glBindVertexArray(vao_);



    // Generate a VBO(Vertex Buffer Object) buffer.
    // This buffer is use to send Vertex data to GPU from CPU.
    glGenBuffers(1, &vertex_vbo_);

    // Bind VBO buffer to GL_ARRAY_BUFFER(array buffer) target.
    // From now on, any function call in this target will action on our VBO buffer.
    glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo_);

    // Copy real vertices data into VBO buffer.
    // param 4:
    // GL_STATIC_DRAW: The data will never or rarely change.
    // GL_DYNAMIC_DRAW：The data will be changed a lot.
    // GL_STREAM_DRAW：The data changes every time it is plotted.
    glBufferData(
        GL_ARRAY_BUFFER, mesh_->getVerticesSize(), mesh_->getVerticesData(), GL_STATIC_DRAW);

    // EBO/IBO (Element Buffer Object/Index Buffer Object)
    glGenBuffers(1, &index_ebo_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_ebo_);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER, mesh_->getIndicesSize(), mesh_->getIndicesData(), GL_STATIC_DRAW);

    // Tell GPU how to decode our vertices data.
    // Set attribute pointer's infomation about our vertices data. (save in VAO)
    // param 1: [int] location index in vertex shader source: 'layout(location = 0)'
    // param 2: [int] size of data, 3 means it's a 'vec3' attribute
    // param 3: [enum] type of data, GL_FLOAT means float (of course..)
    // param 4: [bool] need to normalize or not, GL_TRUE or GL_FALSE
    // param 5: [int] stride, data's stride between each group
    // param 6: [void*] offset, offset of data's begin position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), reinterpret_cast<void*>(0));
    // Make data attribute in location 0 enabled
    // Call the 'glDisableVertexAttribArray' in some where to disabled it.
    glEnableVertexAttribArray(0);



    // VBO for normals, EBO is unnecessary
    glGenBuffers(1, &normal_vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, normal_vbo_);
    glBufferData(GL_ARRAY_BUFFER, mesh_->getNormalsSize(), mesh_->getNormalsData(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(1);
  }

  Model::~Model() {
    glDeleteBuffers(1, &vertex_vbo_);
    glDeleteBuffers(1, &index_ebo_);
    glDeleteBuffers(1, &normal_vbo_);
    glDeleteVertexArrays(1, &vao_);
  }

  auto Model::getMesh() const -> const Mesh* {
    return mesh_;
  }

  auto Model::draw() -> void {
    if (!mesh_) {
      return;
    }

    auto* shader = static_cast<PhongShader*>(shader_);
    if (!shader) {
      return;
    }

    shader->use();

    auto time = static_cast<float>(glfwGetTime());

    auto radians = time * glm::radians(50.0f);
    setModel(glm::rotate(glm::mat4{ 1.0f }, radians, { 0.5f, 1.0f, 0.0f }));
    shader->setModel(getScaledModel());

    auto r = std::sin(time) / 2.0f + 0.3f;
    auto g = std::cos(time) / 2.0f + 0.4f;
    auto b = std::sin(time) / 2.0f + 0.5f;
    shader->setObjectColor({ r, g, b, 1.0f });

    if (auto* camera = scene_->getCamera(); camera) {
      shader->setView(camera->getView());
      shader->setProjection(camera->getProjection());
      shader->setCameraPosition(camera->getPosition());
    }

    if (auto* light = scene_->getLight(); light) {
      shader->setLightColor(light->getColor());
      shader->setLightPosition(light->getPosition());
      shader->setAmbientStrength(light->getAmbientStrength());
      shader->setSpecularStrength(light->getSpecularStrength());
    }

    // Load attributes in VAO
    glBindVertexArray(vao_);

    // Use EBO and VB0 to draw the triangle
    // param 1: [enum] type to draw
    // param 2: [int] index count of element array
    // param 3: [enum] type of index array
    // param 4: [void*] offset of index array
    glDrawElements(
        GL_TRIANGLES, mesh_->getIndicesSize(), GL_UNSIGNED_INT, reinterpret_cast<void*>(0));
  }

}  // namespace brabbit
