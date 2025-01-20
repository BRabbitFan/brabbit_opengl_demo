#include <iostream>
#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <camera.hpp>
#include <model.hpp>
#include <shader.hpp>
#include <scene.hpp>

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



  auto scene = brabbit::Scene{ 50, 50, 50 };

  auto model = std::make_unique<brabbit::Model>("cube.stl"sv);
  auto* object = scene.emplaceObject<brabbit::ModelObject>(model);
  // object->setModel(glm::translate(object->getModel(), { 10.0f, 0.0f, 0.0f }));
  // object->setModel(glm::rotate(object->getModel(), glm::radians(-55.0f), { 1.0f, 0.0f, 0.0f }));



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

    auto* camera = scene.getCamera();

    auto position = camera->position();
    const auto speed = camera->speed() * delta_time;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
      position += camera->front() * speed;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
      position -= camera->front() * speed;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
      position -= camera->right() * speed;
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
      position += camera->right() * speed;
    }

    camera->setPosition(std::move(position));

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
    if (x_offset || y_offset) {
      last_x = cursor_x;
      last_y = cursor_y;

      auto sensitivity = 0.1f;
      x_offset *= sensitivity;
      y_offset *= sensitivity;

      auto yaw = camera->yaw() + x_offset;
      auto pitch = static_cast<float>(camera->pitch() + y_offset);
      pitch = std::max(-89.0f, std::min(89.0f, pitch));

      auto front = glm::vec3{
        std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch)),
        std::sin(glm::radians(pitch)),
        std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch)),
      };

      camera->setFront(glm::normalize(front));
    }
  };

  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  // wireframe mode
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glEnable(GL_DEPTH_TEST);  // enable depth test (use to hide the object behind another object)
  // glClearColor(1.f, 1.f, 1.f, 1.f);  // set clear color
  glClearColor(0.f, 0.f, 0.f, 1.f);  // set clear color

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  // hide cursor and lock it in window

  auto delta_time = 0.0;
  auto last_frame_time = 0.0;
  while (!glfwWindowShouldClose(window)) {
    auto current_frame_time = glfwGetTime();
    delta_time = current_frame_time - last_frame_time;
    last_frame_time = current_frame_time;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    process_window_state(window);
    process_input(window, delta_time);

    scene.drawObjects(screen.projection());

    glfwSwapBuffers(window);  // swap front and back buffers
    glfwPollEvents();  // poll for and process events
  }

  glfwTerminate();
  return 0;
}
