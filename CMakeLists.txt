cmake_minimum_required(VERSION 3.20)

project(brabbit_opengl_demo LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# target : opengl

message("-------------------- configuring opengl --------------------")

find_package(OpenGL REQUIRED)

# # target : glad

message("-------------------- configuring glad --------------------")

file(GLOB_RECURSE GLAD_SOURCES
  ${CMAKE_SOURCE_DIR}/library/glad/src/*
)

add_library(glad STATIC
  ${GLAD_SOURCES}
)

set_target_properties(glad PROPERTIES
  LANGUAGES C
  INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/library/glad/include
)

target_include_directories(glad PRIVATE
  ${CMAKE_SOURCE_DIR}/library/glad/include
)

# target : glfw

message("-------------------- configuring glfw --------------------")

add_library(glfw STATIC IMPORTED)

set_target_properties(glfw PROPERTIES
  IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/library/glfw/lib/glfw3.lib
  INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/library/glfw/include
)

# target : opengl_demo

message("-------------------- configuring opengl_demo --------------------")

file(GLOB_RECURSE OPENGL_DEMO_SOURCES
  ${CMAKE_SOURCE_DIR}/source/*
)

add_executable(opengl_demo
  ${OPENGL_DEMO_SOURCES}
)

target_link_libraries(opengl_demo PRIVATE
  OpenGL::GL
  glad
  glfw
)

target_include_directories(opengl_demo PRIVATE
  ${CMAKE_SOURCE_DIR}/source
)
