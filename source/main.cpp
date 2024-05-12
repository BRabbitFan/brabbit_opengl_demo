#include <GLFW/glfw3.h>

namespace brabbit {

  constexpr auto WIDNOW_DEFAULT_WIDTH{ 800 };
  constexpr auto WIDNOW_DEFAULT_HEIGHT{ 600 };
  constexpr auto WIDNOW_DEFAULT_TITLE{ "BRabbit's OpenGL Demo" };

}  // namespace brabbit

auto main(int argc, char** argv) -> int {
  if (!glfwInit()) {
    return -1;
  }

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

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);  // render here
    glfwSwapBuffers(window);  // swap front and back buffers
    glfwPollEvents();  // poll for and process events
  }

  glfwTerminate();
  return 0;
}
