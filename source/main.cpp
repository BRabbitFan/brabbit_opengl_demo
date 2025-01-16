#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <shader.hpp>

using namespace std::string_literals;
using namespace std::string_view_literals;

auto LoadShader(std::string_view name) -> const GLchar* {
  static auto Cache = std::map<std::string, std::string>{};
  if (auto iter = Cache.find(name.data()); iter != Cache.end()) {
    return iter->second.c_str();
  }

  auto path = std::filesystem::current_path() / "resource"sv / "shader"sv / name;
  if (!std::filesystem::exists(path)) {
    std::cout << "Shader file not found: "sv << path << std::endl;
    return {};
  }

  auto file = std::ifstream{ path };
  if (!file) {
    std::cout << "Open shader file failed: "sv << path << std::endl;
    return {};
  }

  auto source = std::string{ std::istreambuf_iterator<char>{ file },
                             std::istreambuf_iterator<char>{} };
  if (source.empty()) {
    return {};
  }

  return Cache.emplace(name, std::move(source)).first->second.c_str();
}

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
  constexpr auto WIDNOW_WIDTH = 800;
  constexpr auto WIDNOW_HEIGHT = 600;
  constexpr auto WIDNOW_TITLE = "BRabbit's OpenGL Demo"sv;
  auto* const window = glfwCreateWindow(WIDNOW_WIDTH,
                                        WIDNOW_HEIGHT,
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
  glViewport(0, 0, WIDNOW_WIDTH, WIDNOW_HEIGHT);
  glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
  });

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

    const auto center_x = (mode->width - WIDNOW_WIDTH) / 2;
    const auto center_y = (mode->height - WIDNOW_HEIGHT) / 2;
    glfwSetWindowPos(window, center_x, center_y);
  } while (false);

  // glfwSetWindowOpacity(window, .5f);




  // Create a shader program.
  auto shader = brabbit::CubeShader{};



  // Create and bind a VAO(Vertex Array Object)
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);

  // Load attributes in VAO
  // From now on, any function call in this target will action on our VAO buffer.
  glBindVertexArray(VAO);



  // real vertices data
  float vertices[] = {
    // { x, y, z }
    -0.75f, -0.5f, 0.0f,
     0.50f, -0.5f, 0.0f,
    -0.50f,  0.5f, 0.0f,
     0.75f,  0.5f, 0.0f,
  };

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
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);



  // index list/element list
  unsigned int indices[] = {
    0, 1, 2,  // first triangle
    1, 2, 3,  // second triangle
  };

  // EBO/IBO (Element Buffer Object/Index Buffer Object)
  unsigned int EBO;
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);



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
  });



  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  // wireframe mode
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glClearColor(1.f, 1.f, 1.f, 1.f);  // set clear color
  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);  // clear buffer use the color we set before

    // Use the shader program and update uniform variable
    shader.use();
    auto time = glfwGetTime();
    GLfloat r = (std::sin(time) / 2.0f) + 0.3f;
    GLfloat g = (std::cos(time) / 2.0f) + 0.4f;
    GLfloat b = (std::sin(time) / 2.0f) + 0.5f;
    GLfloat a = static_cast<int>(time) % 2 == 0 ? 1.0f : 0.0f;
    shader.setGlobalColor({ r, g, b, a });

    // Load attributes in VAO
    glBindVertexArray(VAO);

    // Use VBO to draw the triangle
    // param 1: [enum] type to draw
    // param 2: [int] first index of vertex array
    // param 3: [int] vertex count
    // glDrawArrays(GL_TRIANGLES, 0, 3);
    // glDrawArrays(GL_TRIANGLES, 1, 3);

    // Use EBO and VB0 to draw the triangle
    // param 1: [enum] type to draw
    // param 2: [int] index count of element array
    // param 3: [enum] type of index array
    // param 4: [void*] offset of index array
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, reinterpret_cast<void*>(0));

    glfwSwapBuffers(window);  // swap front and back buffers
    glfwPollEvents();  // poll for and process events
  }

  glfwTerminate();
  return 0;
}
