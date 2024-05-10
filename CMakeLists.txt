cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 11)
set(PROJECT_NAME "bvh")
set(EXE_NAME "bvh_test")

project(${PROJECT_NAME})

add_executable(${EXE_NAME} src/main.cpp
        src/tiny_obj_loader.cc
)

target_include_directories(${EXE_NAME} PRIVATE include)
