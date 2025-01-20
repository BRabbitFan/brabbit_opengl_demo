#include <algorithm>
#include <array>
#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <camera.hpp>
#include <scene.hpp>
#include <shader.hpp>

namespace brabbit {

  namespace {

    template <typename _Type>
    auto GenerateContainsChecker(const _Type* object) {
      return [object](const std::unique_ptr<_Type>& obj) { return obj.get() == object; };
    }

  }  // namespace

  Scene::Scene() : Scene{ 1, 1, 1 } {}

  Scene::Scene(std::size_t width, std::size_t height, std::size_t depth)
      : width_{ width }, height_{ height }, depth_{ depth } , camera_{std::make_unique<Camera>()} {
    updateScaleFactor();
    light_ = emplaceObject<LightCube>();
  }

  Scene::~Scene() {}

  auto Scene::getWidth() const -> std::size_t {
    return width_;
  }

  auto Scene::setWidth(std::size_t width) -> void {
    width_ = width;
    updateScaleFactor();
  }

  auto Scene::getHeight() const -> std::size_t {
    return height_;
  }

  auto Scene::setHeight(std::size_t height) -> void {
    height_ = height;
    updateScaleFactor();
  }

  auto Scene::getDepth() const -> std::size_t {
    return depth_;
  }

  auto Scene::setDepth(std::size_t depth) -> void {
    depth_ = depth;
    updateScaleFactor();
  }

  auto Scene::getScaleFactor() const -> double {
    return scale_factor_;
  }

  auto Scene::objects() const -> const std::vector<std::unique_ptr<SceneObject>>& {
    return objects_;
  }

  auto Scene::containsObject(const SceneObject* object) const -> bool {
    return std::any_of(objects_.cbegin(), objects_.cend(), GenerateContainsChecker(object));
  }

  auto Scene::emplaceObject(std::unique_ptr<SceneObject>&& object) -> SceneObject* {
    if (!object) {
      return nullptr;
    }

    object->scene_ = this;
    return objects_.emplace_back(std::move(object)).get();
  }

  auto Scene::eraseObject(const SceneObject* object) -> void {
    objects_.erase(
        std::remove_if(objects_.begin(), objects_.end(), GenerateContainsChecker(object)),
        objects_.end());
  }

  auto Scene::takeObject(const SceneObject* object) -> std::unique_ptr<SceneObject> {
    auto iter = std::find_if(objects_.begin(), objects_.end(), GenerateContainsChecker(object));
    if (iter == objects_.end()) {
      return nullptr;
    }

    auto obj = std::move(*iter);
    objects_.erase(iter);
    return obj;
  }

  auto Scene::drawObjects(const glm::mat4& projection) -> void {
    for (const auto& object : objects_) {
      if (object) {
        object->draw(camera_->view(), projection);
      }
    }
  }

  auto Scene::getCamera() const -> const Camera* {
    return camera_.get();
  }

  auto Scene::getCamera() -> Camera* {
    return camera_.get();
  }

  auto Scene::getLightCube() const -> const LightCube* {
    return light_;
  }

  auto Scene::getLightCube() -> LightCube* {
    return light_;
  }

  auto Scene::updateScaleFactor() -> void {
    scale_factor_ = 1.0 / static_cast<double>(std::max({ width_, height_, depth_ }));
  }



  SceneObject::SceneObject() {

  }

  SceneObject::~SceneObject() {

  }

  auto SceneObject::getModel() const -> const glm::mat4& {
    return model_;
  }

  auto SceneObject::setModel(const glm::mat4& model) -> void {
    model_ = model;
  }

  auto SceneObject::setModel(glm::mat4&& model) -> void {
    model_ = std::move(model);
  }

  auto SceneObject::getScaledModel() const -> glm::mat4 {
    auto factor = scene_->getScaleFactor();
    return glm::scale(model_, { factor, factor, factor });
  }

  auto SceneObject::getShader() const -> const Shader* {
    return shader_;
  }

  auto SceneObject::getShader() -> Shader* {
    return shader_;
  }

  auto SceneObject::setShader(Shader* shader) -> void {
    shader_ = shader;
  }

  auto SceneObject::getScene() const -> const Scene* {
    return scene_;
  }

  auto SceneObject::getScene() -> Scene* {
    return scene_;
  }


  namespace {

    constexpr auto LIGHT_WIDTH = 2.0f;
    constexpr auto HALF_LIGHT_WIDTH = LIGHT_WIDTH / 2.0f;

    constexpr auto LIGHT_VERTICES = std::array<GLfloat, 24>{
      -HALF_LIGHT_WIDTH, -HALF_LIGHT_WIDTH, -HALF_LIGHT_WIDTH,
       HALF_LIGHT_WIDTH, -HALF_LIGHT_WIDTH, -HALF_LIGHT_WIDTH,
      -HALF_LIGHT_WIDTH,  HALF_LIGHT_WIDTH, -HALF_LIGHT_WIDTH,
       HALF_LIGHT_WIDTH,  HALF_LIGHT_WIDTH, -HALF_LIGHT_WIDTH,
      -HALF_LIGHT_WIDTH, -HALF_LIGHT_WIDTH,  HALF_LIGHT_WIDTH,
       HALF_LIGHT_WIDTH, -HALF_LIGHT_WIDTH,  HALF_LIGHT_WIDTH,
      -HALF_LIGHT_WIDTH,  HALF_LIGHT_WIDTH,  HALF_LIGHT_WIDTH,
       HALF_LIGHT_WIDTH,  HALF_LIGHT_WIDTH,  HALF_LIGHT_WIDTH,
    };
    constexpr auto LIGHT_VERTICES_SIZE = LIGHT_VERTICES.size() * sizeof(GLfloat);
    constexpr auto LIGHT_VERTICES_DATA = LIGHT_VERTICES.data();
    constexpr auto LIGHT_VERTICES_STRIDE = 3 * sizeof(GLfloat);

    constexpr auto LIGHT_INDICES = std::array<GLuint, 36>{
      0, 1, 2,  1, 3, 2,  // front
      4, 6, 5,  5, 6, 7,  // back
      0, 2, 4,  2, 6, 4,  // left
      1, 3, 5,  3, 7, 5,  // right
      0, 4, 1,  1, 4, 5,  // bottom
      2, 3, 6,  3, 7, 6,  // top
    };
    constexpr auto LIGHT_INDICES_SIZE = LIGHT_INDICES.size() * sizeof(GLuint);
    constexpr auto LIGHT_INDICES_DATA = LIGHT_INDICES.data();

  }  // namespace


  LightCube::LightCube() {
    model_ = glm::translate(glm::mat4{ 1.0f }, { 1.0f, 1.0f, 1.0f });
    shader_ = LoadCachedShader<LightCubeShader>();

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
    glBufferData(GL_ARRAY_BUFFER, LIGHT_VERTICES_SIZE, LIGHT_VERTICES_DATA, GL_STATIC_DRAW);

    // EBO/IBO (Element Buffer Object/Index Buffer Object)
    glGenBuffers(1, &ebo_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, LIGHT_INDICES_SIZE, LIGHT_INDICES_DATA, GL_STATIC_DRAW);

    // Tell GPU how to decode our vertices data.
    // Set attribute pointer's infomation about our vertices data. (save in VAO)
    // param 1: [int] location index in vertex shader source: 'layout(location = 0)'
    // param 2: [int] size of data, 3 means it's a 'vec3' attribute
    // param 3: [enum] type of data, GL_FLOAT means float (of course..)
    // param 4: [bool] need to normalize or not, GL_TRUE or GL_FALSE
    // param 5: [int] stride, data's stride between each group
    // param 6: [void*] offset, offset of data's begin position
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, LIGHT_VERTICES_STRIDE, reinterpret_cast<void*>(0));
    // Make data attribute in location 0 enabled
    // Call the 'glDisableVertexAttribArray' in some where to disabled it.
    glEnableVertexAttribArray(0);
  }

  LightCube::~LightCube() {

  }

  auto LightCube::getColor() const -> const glm::vec4& {
    return color_;
  }

  auto LightCube::setColor(const glm::vec4& color) -> void {
    color_ = color;
  }

  auto LightCube::draw(const glm::mat4& view, const glm::mat4& projection) -> void {
    auto* shader = static_cast<LightCubeShader*>(shader_);

    shader->use();

    shader->setModel(getScaledModel());
    shader->setView(view);
    shader->setProjection(projection);
    shader->setLightColor(color_);

    // Load attributes in VAO
    glBindVertexArray(vao_);

    // Use EBO and VB0 to draw the triangle
    // param 1: [enum] type to draw
    // param 2: [int] index count of element array
    // param 3: [enum] type of index array
    // param 4: [void*] offset of index array
    glDrawElements(GL_TRIANGLES, LIGHT_INDICES_SIZE, GL_UNSIGNED_INT, reinterpret_cast<void*>(0));
  }

}  // namespace brabbit
