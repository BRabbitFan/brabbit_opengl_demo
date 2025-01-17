#include <chrono>
#include <thread>
#include <iostream>

#include "glm/detail/qualifier.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <model.hpp>
#include <shader.hpp>

using namespace std::string_literals;
using namespace std::string_view_literals;

auto main(int argc, char** argv) -> int {
  // Init the context.
  if (!glfwInit()) {
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif



  // Create a window.
  static auto screen = brabbit::Screen{ 800, 600 };
  constexpr auto WIDNOW_TITLE = "BRabbit's OpenGL Demo"sv;
  auto* const window = glfwCreateWindow(screen.width(),
                                        screen.height(),
                                        WIDNOW_TITLE.data(),
                                        nullptr,
                                        nullptr);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    glfwTerminate();
    return -1;
  }

  if (const auto* version = glGetString(GL_VERSION); version) {
    std::cout << "OpenGL version: "sv << version << std::endl;
  }

  // Init the OpenGL view area, at this call it will also init the window's size.
  // Then we set a callback. Reset the OpenGL view area as window's size when window's size changed.
  constexpr auto update_viewport = [](GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    screen.setSize(width, height);
  };
  update_viewport(window, screen.width(), screen.height());
  glfwSetFramebufferSizeCallback(window, update_viewport);

  // Init the window's position at center of primary monitor.
  do {
    auto* const monitor = glfwGetPrimaryMonitor();
    if (!monitor) {
      break;
    }

    const auto* const mode = glfwGetVideoMode(monitor);
    if (!mode) {
      break;
    }

    const auto center_x = (mode->width - screen.width()) / 2;
    const auto center_y = (mode->height - screen.height()) / 2;
    glfwSetWindowPos(window, center_x, center_y);
  } while (false);

  // glfwSetWindowOpacity(window, .5f);




  // Create a shader program.
  auto shader = brabbit::CubeShader{};


  static auto camera = brabbit::Camera{};
  camera.view_ = glm::translate(camera.view_, { 0.0f, 0.0f, -3.0f });

  auto model = brabbit::Model{ "cube.stl"sv };
  model.setModel(glm::rotate(model.model(), glm::radians(-55.0f), { 1.0f, 0.0f, 0.0f }));



  // Create and bind a VAO(Vertex Array Object)
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);

  // Load attributes in VAO
  // From now on, any function call in this target will action on our VAO buffer.
  glBindVertexArray(VAO);



  // Generate a VBO(Vertex Buffer Object) buffer.
  // This buffer is use to send Vertex data to GPU from CPU.
  unsigned int VBO;
  glGenBuffers(1, &VBO);

  // Bind VBO buffer to GL_ARRAY_BUFFER(array buffer) target.
  // From now on, any function call in this target will action on our VBO buffer.
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  // Copy real vertices data into VBO buffer.
  // param 4:
  // GL_STATIC_DRAW: The data will never or rarely change.
  // GL_DYNAMIC_DRAW：The data will be changed a lot.
  // GL_STREAM_DRAW：The data changes every time it is plotted.
  glBufferData(GL_ARRAY_BUFFER, model.verticesSize(), model.verticesData(), GL_STATIC_DRAW);



  // EBO/IBO (Element Buffer Object/Index Buffer Object)
  unsigned int EBO;
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.indicesSize(), model.indicesData(), GL_STATIC_DRAW);



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



  glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, true);
      return;
    }

    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
      auto front = glm::normalize(glm::vec3{ camera.view_[0][2], camera.view_[1][2], camera.view_[2][2] });
      camera.view_ = glm::translate(camera.view_, front);
    }

    if (key == GLFW_KEY_S && action == GLFW_PRESS) {
      auto back = glm::normalize(glm::vec3{ -camera.view_[0][2], -camera.view_[1][2], -camera.view_[2][2] });
      camera.view_ = glm::translate(camera.view_, back);
    }

    if (key == GLFW_KEY_A && action == GLFW_PRESS) {
      auto right = glm::normalize(glm::vec3{ camera.view_[0][0], camera.view_[1][0], camera.view_[2][0] });
      camera.view_ = glm::translate(camera.view_, right);
    }

    if (key == GLFW_KEY_D && action == GLFW_PRESS) {
      auto left = glm::normalize(glm::vec3{ -camera.view_[0][0], -camera.view_[1][0], -camera.view_[2][0] });
      camera.view_ = glm::translate(camera.view_, left);
    }
  });



  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  // wireframe mode
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glEnable(GL_DEPTH_TEST);  // enable depth test (use to hide the object behind another object)
  glClearColor(1.f, 1.f, 1.f, 1.f);  // set clear color
  while (!glfwWindowShouldClose(window)) {
    auto start = std::chrono::steady_clock::now();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use the shader program and update uniform variable

    shader.use();

    auto time = glfwGetTime();

    GLfloat r = std::sin(time) / 2.0f + 0.3f;
    GLfloat g = std::cos(time) / 2.0f + 0.4f;
    GLfloat b = std::sin(time) / 2.0f + 0.5f;
    GLfloat a = static_cast<int>(time) % 2 == 0 ? 1.0f : 0.0f;
    shader.setGlobalColor({ r, g, b, a });

    auto radians = static_cast<float>(time) * glm::radians(50.0f);
    model.setModel(glm::rotate(glm::mat4{ 1.0f }, radians, { 0.5f, 1.0f, 0.0f }));

    shader.setModel(model.model());
    shader.setView(camera.view_);
    shader.setProjection(screen.projection());

    // Load attributes in VAO
    glBindVertexArray(VAO);

    // Use EBO and VB0 to draw the triangle
    // param 1: [enum] type to draw
    // param 2: [int] index count of element array
    // param 3: [enum] type of index array
    // param 4: [void*] offset of index array
    glDrawElements(GL_TRIANGLES, model.indicesSize(), GL_UNSIGNED_INT, reinterpret_cast<void*>(0));

    glfwSwapBuffers(window);  // swap front and back buffers
    glfwPollEvents();  // poll for and process events

    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1000ms / 119 - duration);
  }

  glfwTerminate();
  return 0;
}
