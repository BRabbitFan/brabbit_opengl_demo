#include <iostream>

#include "glm/detail/qualifier.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <camera.hpp>
#include <model.hpp>
#include <shader.hpp>
#include <threads.h>

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
  auto screen = brabbit::Screen{ 800, 600 };
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
  glViewport(0, 0, screen.width(), screen.height());

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

  auto camera = brabbit::Camera{};

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



  const auto process_window_state = [&](GLFWwindow* window) {
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    if (width != screen.width() || height != screen.height()) {
      glViewport(0, 0, width, height);
      screen.setSize(width, height);
    }
  };

  const auto process_input = [&](GLFWwindow* window, float delta_time) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, true);
    }

    const auto speed = camera.speed() * delta_time;
    const auto& front = camera.front();
    const auto& up = camera.up();

    auto position = camera.position();

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
      position += camera.front() * speed;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
      position -= camera.front() * speed;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
      position -= camera.right() * speed;
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
      position += camera.right() * speed;
    }

    camera.setPosition(std::move(position));

    static auto last_x = -1.0;
    static auto last_y = -1.0;
    auto cursor_x = 0.0;
    auto cursor_y = 0.0;
    glfwGetCursorPos(window, &cursor_x, &cursor_y);

    if (last_x < 0.0 || last_y < 0.0) {
      last_x = cursor_x;
      last_y = cursor_y;
    }

    auto x_offset = cursor_x - last_x;
    auto y_offset = last_y - cursor_y;  // reversed since y-coordinates go from bottom to top
    if (x_offset > 0.0 || y_offset > 0.0) {
      last_x = cursor_x;
      last_y = cursor_y;

      auto sensitivity = 0.1f;
      x_offset *= sensitivity;
      y_offset *= sensitivity;

      auto yaw = camera.yaw() + x_offset;
      // auto pitch = camera.pitch() + y_offset;
      auto pitch = std::max(-89.0f, std::min(89.0f, static_cast<float>(camera.pitch() + y_offset)));

      auto front = glm::vec3{
        std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch)),
        std::sin(glm::radians(pitch)),
        std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch)),
      };

      camera.setFront(glm::normalize(front));
    }
  };

  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  // wireframe mode
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glEnable(GL_DEPTH_TEST);  // enable depth test (use to hide the object behind another object)
  glClearColor(1.f, 1.f, 1.f, 1.f);  // set clear color

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  // hide cursor and lock it in window

  auto delta_time = 0.0;
  auto last_frame_time = 0.0;
  while (!glfwWindowShouldClose(window)) {
    auto current_frame_time = glfwGetTime();
    delta_time = current_frame_time - last_frame_time;
    last_frame_time = current_frame_time;

    process_window_state(window);
    process_input(window, delta_time);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use the shader program and update uniform variable

    shader.use();

    GLfloat r = std::sin(current_frame_time) / 2.0f + 0.3f;
    GLfloat g = std::cos(current_frame_time) / 2.0f + 0.4f;
    GLfloat b = std::sin(current_frame_time) / 2.0f + 0.5f;
    GLfloat a = static_cast<int>(current_frame_time) % 2 == 0 ? 1.0f : 0.0f;
    shader.setGlobalColor({ r, g, b, a });

    auto radians = static_cast<float>(current_frame_time) * glm::radians(50.0f);
    model.setModel(glm::rotate(glm::mat4{ 1.0f }, radians, { 0.5f, 1.0f, 0.0f }));

    shader.setModel(model.model());
    shader.setView(camera.view());
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
  }

  glfwTerminate();
  return 0;
}
