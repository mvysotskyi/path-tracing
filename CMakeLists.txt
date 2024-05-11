cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 11)
set(PROJECT_NAME "pathtrace")
set(EXE_NAME "pathtracer")

project(${PROJECT_NAME})

add_executable(${EXE_NAME} src/main.cpp
        src/tiny_obj_loader.cc
        src/shader.cpp
        src/compute_shader.cpp
)

target_include_directories(${EXE_NAME} PRIVATE include)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)

target_link_libraries(${EXE_NAME} OpenGL::GL glfw GLEW::GLEW glm::glm OpenGL::GLU)