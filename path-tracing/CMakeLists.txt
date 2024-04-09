cmake_minimum_required(VERSION 3.2)
project(path_tracing)

set(CMAKE_CXX_STANDARD 17)

add_executable(path_tracing main.cpp
        compute_shader.cpp
        shader.cpp)

target_include_directories(path_tracing PRIVATE include)

# OpenGL, GLFW, GLEW, glm
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)

target_link_libraries(path_tracing OpenGL::GL glfw GLEW::GLEW glm::glm)
