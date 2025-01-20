#include <array>

#include <brabbit/light.hpp>
#include <brabbit/scene.hpp>
#include <brabbit/flat_shader.hpp>

namespace brabbit {

  namespace {

    constexpr auto WIDTH = 2.0f;
    constexpr auto HALF_WIDTH = WIDTH / 2.0f;

    constexpr auto VERTICES = std::array<GLfloat, 24>{
      -HALF_WIDTH, -HALF_WIDTH, -HALF_WIDTH,
       HALF_WIDTH, -HALF_WIDTH, -HALF_WIDTH,
      -HALF_WIDTH,  HALF_WIDTH, -HALF_WIDTH,
       HALF_WIDTH,  HALF_WIDTH, -HALF_WIDTH,
      -HALF_WIDTH, -HALF_WIDTH,  HALF_WIDTH,
       HALF_WIDTH, -HALF_WIDTH,  HALF_WIDTH,
      -HALF_WIDTH,  HALF_WIDTH,  HALF_WIDTH,
       HALF_WIDTH,  HALF_WIDTH,  HALF_WIDTH,
    };
    constexpr auto VERTICES_SIZE = VERTICES.size() * sizeof(GLfloat);
    constexpr auto VERTICES_DATA = VERTICES.data();
    constexpr auto VERTICES_STRIDE = 3 * sizeof(GLfloat);

    constexpr auto INDICES = std::array<GLuint, 36>{
      0, 1, 2,  1, 3, 2,  // front
      4, 6, 5,  5, 6, 7,  // back
      0, 2, 4,  2, 6, 4,  // left
      1, 3, 5,  3, 7, 5,  // right
      0, 4, 1,  1, 4, 5,  // bottom
      2, 3, 6,  3, 7, 6,  // top
    };
    constexpr auto INDICES_SIZE = INDICES.size() * sizeof(GLuint);
    constexpr auto INDICES_DATA = INDICES.data();

  }  // namespace



  Light::Light() : position_{ 1.0f, 1.0f, 1.0f }, color_{ 1.0f, 1.0f, 1.0f, 1.0f } {
    updateModel();
    shader_ = LoadCachedShader<FlatShader>();

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
    glBufferData(GL_ARRAY_BUFFER, VERTICES_SIZE, VERTICES_DATA, GL_STATIC_DRAW);

    // EBO/IBO (Element Buffer Object/Index Buffer Object)
    glGenBuffers(1, &ebo_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, INDICES_SIZE, INDICES_DATA, GL_STATIC_DRAW);

    // Tell GPU how to decode our vertices data.
    // Set attribute pointer's infomation about our vertices data. (save in VAO)
    // param 1: [int] location index in vertex shader source: 'layout(location = 0)'
    // param 2: [int] size of data, 3 means it's a 'vec3' attribute
    // param 3: [enum] type of data, GL_FLOAT means float (of course..)
    // param 4: [bool] need to normalize or not, GL_TRUE or GL_FALSE
    // param 5: [int] stride, data's stride between each group
    // param 6: [void*] offset, offset of data's begin position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTICES_STRIDE, reinterpret_cast<void*>(0));
    // Make data attribute in location 0 enabled
    // Call the 'glDisableVertexAttribArray' in some where to disabled it.
    glEnableVertexAttribArray(0);
  }

  Light::~Light() {

  }

  auto Light::isLampVisible() const -> bool {
    return lamp_visible_;
  }

  auto Light::setLampVisible(bool visible) -> void {
    lamp_visible_ = visible;
  }

  auto Light::getColor() const -> const glm::vec4& {
    return color_;
  }

  auto Light::setColor(const glm::vec4& color) -> void {
    color_ = color;
  }

  auto Light::getPosition() const -> const glm::vec3& {
    return position_;
  }

  auto Light::setPosition(const glm::vec3& position) -> void {
    position_ = position;
    updateModel();
  }

  auto Light::getAmbientStrength() const -> float {
    return ambient_strength_;
  }

  auto Light::setAmbientStrength(float ambient_strength) -> void {
    ambient_strength_ = ambient_strength;
  }

  auto Light::getSpecularStrength() const -> float {
    return specular_strength_;
  }

  auto Light::setSpecularStrength(float specular_strength) -> void {
    specular_strength_ = specular_strength;
  }

  auto Light::draw() -> void {
    if (!lamp_visible_) {
      return;
    }

    auto* shader = static_cast<FlatShader*>(shader_);
    if (!shader) {
      return;
    }

    auto* camera = scene_->getCamera();
    if (!camera) {
      return;
    }

    shader->use();
    shader->setModel(getScaledModel());
    shader->setView(camera->getView());
    shader->setProjection(camera->getProjection());
    shader->setLightColor(color_);

    // Load attributes in VAO
    glBindVertexArray(vao_);

    // Use EBO and VB0 to draw the triangle
    // param 1: [enum] type to draw
    // param 2: [int] index count of element array
    // param 3: [enum] type of index array
    // param 4: [void*] offset of index array
    glDrawElements(GL_TRIANGLES, INDICES_SIZE, GL_UNSIGNED_INT, reinterpret_cast<void*>(0));
  }

  auto Light::updateModel() -> void {
    model_ = glm::translate(glm::mat4{ 1.0f }, position_);
  }

}  // namespace brabbit
