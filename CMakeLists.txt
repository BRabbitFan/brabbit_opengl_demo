cmake_minimum_required(VERSION 3.16)

project(brabbit_opengl_demo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# target : opengl

find_package(OpenGL REQUIRED)

# target : glfw

add_library(glfw STATIC IMPORTED)
set_target_properties(glfw PROPERTIES
  IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/library/glfw/lib/glfw3.lib
  INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/library/glfw/include
)

# target : opengl_demo

add_executable(opengl_demo
  source/main.cpp
)

target_link_libraries(opengl_demo PRIVATE
  glfw
  OpenGL::GL
)

target_include_directories(opengl_demo PRIVATE
  ${CMAKE_SOURCE_DIR}/source
)
