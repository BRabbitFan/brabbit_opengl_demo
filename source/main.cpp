#include <iostream>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <GL/gl.h>

namespace brabbit {

  constexpr auto WIDNOW_DEFAULT_WIDTH{ 800 };
  constexpr auto WIDNOW_DEFAULT_HEIGHT{ 600 };
  constexpr auto WIDNOW_DEFAULT_TITLE{ "BRabbit's OpenGL Demo" };

  constexpr auto VERTEX_SHADER_SOURCE{
    // require OpenGL version 4.6.0
    "#version 460 core\n"
    // get input into vec3(Type) aPos(name) in location 0
    "layout (location = 0) in vec3 aPos;\n"
    "void main() {\n"
    // set output into gl_Position(pre defined variant)
    "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}"
  };

  constexpr auto FRAGMENT_SHADER_SOURCE{
    "#version 460 core\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"  // an orange color
    "}"
  };

}  // namespace brabbit

auto main(int argc, char** argv) -> int {
  // Init the context.
  if (!glfwInit()) {
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif



  // Create a window.
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

  // Init the OpenGL view area, at this call it will also init the window's size.
  // Then we set a callback. Reset the OpenGL view area as window's size when window's size changed.
  glViewport(0, 0, brabbit::WIDNOW_DEFAULT_WIDTH, brabbit::WIDNOW_DEFAULT_HEIGHT);
  glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
  });

  // Init the window's position at center of primary monitor.
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



  // Create a vertex shader object.
  auto vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  // Set shader's source and compile it.
  // param 2: source string count
  glShaderSource(vertex_shader, 1, &brabbit::VERTEX_SHADER_SOURCE, NULL);
  glCompileShader(vertex_shader);

  // check if compile successed.
  int successed;
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &successed);
  if (!successed) {
    char message[512];
    glGetShaderInfoLog(vertex_shader, sizeof(message), NULL, message);
    std::cout << "[error] create vertex shader object error :\n  " << message << std::endl;
  }



  // Create a fragment shader object, just like vertex shader object.
  auto fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &brabbit::FRAGMENT_SHADER_SOURCE, NULL);
  glCompileShader(fragment_shader);

  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &successed);
  if (!successed) {
    char message[512];
    glGetShaderInfoLog(fragment_shader, sizeof(message), NULL, message);
    std::cout << "[error] create fragment shader object error :\n  " << message << std::endl;
  }



  // Create a shader program object, and link the vertex shader and the fragment shader object into.
  unsigned int shader_program;
  shader_program = glCreateProgram();
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  glLinkProgram(shader_program);

  glGetProgramiv(shader_program, GL_LINK_STATUS, &successed);
  if (!successed) {
    char message[512];
    glGetProgramInfoLog(shader_program, sizeof(message), NULL, message);
    std::cout << "[error] create shader program object error :\n  " << message << std::endl;
  }

  // shader_program allready got the shaders, we don't need then anymore.
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);



  // Create and bind a VAO(Vertex Array Object)
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  // Load attributes in VAO
  glBindVertexArray(VAO);



  // real vertices data
  float vertices[] = {
    // { x, y, z }
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f,
     0.5f,  0.5f, 0.0f,
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



  // index list
  unsigned int indices[] = {
    0, 1, 2,  // first triangle
    1, 2, 3,  // second triangle
  };

  // EBO (Element Buffer Object)
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
  // param 6: [(void*)int] offset, offset of data's begin position
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  // Make data attribute in location 0 enabled
  // Call the 'glDisableVertexAttribArray' in some where to disabled it.
  glEnableVertexAttribArray(0);



  glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, true);
      return;
    }
  });

  while (!glfwWindowShouldClose(window)) {
    glClearColor(1.f, 1.f, 1.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);  // render here

    // The shader program object created allready, let's use it!
    // Actrualy action here: our shaders will send to GPU.
    glUseProgram(shader_program);

    // Load attributes in VAO
    glBindVertexArray(VAO);

    // Draw the triangle
    // param 1: [enum] type to draw
    // param 2: [int] first index of vertex array
    // param 3: [int] vertex count
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);  // swap front and back buffers
    glfwPollEvents();  // poll for and process events
  }

  glfwTerminate();
  return 0;
}
