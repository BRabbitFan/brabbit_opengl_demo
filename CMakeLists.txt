cmake_minimum_required(VERSION 3.20)

project(brabbit_opengl_demo LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# target : resource

message("-------------------- configuring resource --------------------")

add_custom_target(resource
  DEPENDS ${CMAKE_SOURCE_DIR}/resource
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resource ${CMAKE_BINARY_DIR}/resource
)

# target : opengl

message("-------------------- configuring opengl --------------------")

find_package(OpenGL REQUIRED)

# target : glad

message("-------------------- configuring glad --------------------")

file(GLOB_RECURSE GLAD_SOURCES
  ${CMAKE_SOURCE_DIR}/library/glad/src/*
)

add_library(glad STATIC
  ${GLAD_SOURCES}
)

add_library(glad::glad ALIAS glad)

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
add_library(glfw::glfw ALIAS glfw)

set_target_properties(glfw PROPERTIES
  IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/library/glfw/lib/glfw3.lib
  INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/library/glfw/include
)

# target : glm

message("-------------------- configuring glm --------------------")

# cmake_policy(SET CMP0077 NEW)
option(GLM_BUILD_LIBRARY OFF)
option(GLM_ENABLE_CXX_20 ON)
add_subdirectory(${CMAKE_SOURCE_DIR}/library/glm)

# target : opengl_demo

message("-------------------- configuring opengl_demo --------------------")

file(GLOB_RECURSE OPENGL_DEMO_SOURCES
  ${CMAKE_SOURCE_DIR}/source/*
)

add_executable(opengl_demo
  ${OPENGL_DEMO_SOURCES}
)

add_dependencies(opengl_demo
  resource
)

target_link_libraries(opengl_demo PRIVATE
  OpenGL::GL
  glad::glad
  glfw::glfw
  glm::glm-header-only
)

target_include_directories(opengl_demo PRIVATE
  ${CMAKE_SOURCE_DIR}/source
)
