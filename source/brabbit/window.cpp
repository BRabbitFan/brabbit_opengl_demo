#include <brabbit/window.hpp>

namespace brabbit {

  using namespace std::string_literals;
  using namespace std::string_view_literals;

  Window::Window(std::string_view title) : Window{ title, 800, 600 } {}

  Window::Window(std::size_t width, std::size_t height) : Window{ ""sv, width, height } {}

  Window::Window(std::string_view title, std::size_t width, std::size_t height)
      : title_{ title }, width_{ width }, height_{ height } {
    if (!glfwInit()) {
      return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

    handle_ = glfwCreateWindow(width_, height_, title_.c_str(), nullptr, nullptr);
    if (!handle_) {
      return;
    }

    glfwMakeContextCurrent(handle_);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      return;
    }

    if (const auto* version = glGetString(GL_VERSION); version) {
      // std::cout << "OpenGL version: "sv << version << std::endl;
    }

    // Init the OpenGL view area, at this call it will also init the window's size.
    glViewport(0, 0, width_, height_);

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

      const auto center_x = (mode->width - width_) / 2;
      const auto center_y = (mode->height - height_) / 2;
      glfwSetWindowPos(handle_, center_x, center_y);
    } while (false);

    // glfwSetWindowOpacity(window, .5f);
    glfwSetInputMode(handle_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  // fps style cursor action

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  // wireframe mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glEnable(GL_DEPTH_TEST);  // enable depth test (use to hide the object behind another object)
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);  // set clear color
  }

  Window::~Window() {
    if (handle_) {
      glfwDestroyWindow(handle_);
    }

    glfwTerminate();
  }

  auto Window::isVaild() const -> bool {
    return handle_;
  }

  auto Window::getHandle() const -> GLFWwindow* {
    return handle_;
  }

  auto Window::getTitle() const -> std::string {
    return title_;
  }

  auto Window::setTitle(std::string_view title) -> void {
    title_ = title;
    glfwSetWindowTitle(handle_, title_.c_str());
  }

  auto Window::getWidth() const -> std::size_t {
    return width_;
  }

  auto Window::setWidth(std::size_t width) -> void {
    width_ = width;
    glfwSetWindowSize(handle_, width_, height_);
  }

  auto Window::getHeight() const -> std::size_t {
    return height_;
  }

  auto Window::setHeight(std::size_t height) -> void {
    height_ = height;
    glfwSetWindowSize(handle_, width_, height_);
  }

  auto Window::getSize() const -> glm::vec2 {
    return { width_, height_ };
  }

  auto Window::setSize(const glm::vec2& size) -> void {
    width_ = size.x;
    height_ = size.y;
    glfwSetWindowSize(handle_, width_, height_);
  }

  auto Window::getScene() const -> const Scene* {
    return scene_.get();
  }

  auto Window::getScene() -> Scene* {
    return scene_.get();
  }

  auto Window::setScene(std::unique_ptr<Scene>&& scene) -> Scene* {
    if (scene_) {
      scene_->setWindow(nullptr);
    }

    scene_ = std::move(scene);

    if (scene_) {
      scene_->setWindow(this);
      if (auto* camera = scene_->getCamera(); camera) {
        camera->setSize({ width_, height_ });
      }
    }

    return scene_.get();
  }

  auto Window::executeLoop() -> int {
    if (!handle_ || !scene_) {
      return -1;
    }

    const auto update_window_size = [this]() {
      int width, height;
      glfwGetWindowSize(handle_, &width, &height);

      if (width != width_ || height != height_) {
        width_ = width;
        height_ = height;
        glViewport(0, 0, width_, height_);

        if (scene_) {
          auto* camera = scene_->getCamera();
          if (camera) {
            camera->setSize({ width_, height_ });
          }
        }
      }
    };

    auto delta = 0.0;
    auto prev = 0.0;
    while (!glfwWindowShouldClose(handle_)) {
      auto curr = glfwGetTime();
      delta = curr - prev;
      prev = curr;

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      update_window_size();
      scene_->processFrame(delta);

      // process_input(handle_, delta_time);
      // scene->drawObjects();

      glfwSwapBuffers(handle_);  // swap front and back buffers
      glfwPollEvents();  // poll for and process events
    }

    return 0;
  }

}  // namespace brabbit
