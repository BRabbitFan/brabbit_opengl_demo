#include <iostream>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <GL/gl.h>

namespace brabbit {

  constexpr auto WIDNOW_DEFAULT_WIDTH{ 800 };
  constexpr auto WIDNOW_DEFAULT_HEIGHT{ 600 };
  constexpr auto WIDNOW_DEFAULT_TITLE{ "BRabbit's OpenGL Demo" };

}  // namespace brabbit

auto main(int argc, char** argv) -> int {
  if (!glfwInit()) {
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

  auto* const window = glfwCreateWindow(brabbit::WIDNOW_DEFAULT_WIDTH,
                                        brabbit::WIDNOW_DEFAULT_HEIGHT,
                                        brabbit::WIDNOW_DEFAULT_TITLE,
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
    std::cout << "OpenGL version: " << version << std::endl;
  }

  glViewport(0, 0, brabbit::WIDNOW_DEFAULT_WIDTH, brabbit::WIDNOW_DEFAULT_HEIGHT);
  glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
  });

  do {
    auto* const monitor = glfwGetPrimaryMonitor();
    if (!monitor) {
      break;
    }

    const auto* const monitor_video_mode = glfwGetVideoMode(monitor);
    if (!monitor_video_mode) {
      break;
    }

    glfwSetWindowPos(window,
                    (monitor_video_mode->width - brabbit::WIDNOW_DEFAULT_WIDTH) / 2,
                    (monitor_video_mode->height - brabbit::WIDNOW_DEFAULT_HEIGHT) / 2);
  } while (false);

  // glfwSetWindowOpacity(window, .5f);

  glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, true);
    }
  });

  while (!glfwWindowShouldClose(window)) {
    glClearColor(1.f, 1.f, 1.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);  // render here
    glfwSwapBuffers(window);  // swap front and back buffers
    glfwPollEvents();  // poll for and process events
  }

  glfwTerminate();
  return 0;
}
