//
// Created by mykola on 08.04.24.
//

#include <fstream>
#include <iostream>
#include <GL/glew.h>

#include "shader.h"

shader::shader(const std::string& vertex_path, const std::string& fragment_path) {
    std::ifstream vertex_file(vertex_path);

    if (!vertex_file.is_open()) {
        std::cerr << "Failed to open file: " << vertex_path << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream fragment_file(fragment_path);

    if (!fragment_file.is_open()) {
        std::cerr << "Failed to open file: " << fragment_path << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string vertex_source;
    std::string fragment_source;

    std::string line;

    while (std::getline(vertex_file, line)) {
        vertex_source += line + '\n';
    }

    while (std::getline(fragment_file, line)) {
        fragment_source += line + '\n';
    }

    const char* c_vertex_source = vertex_source.c_str();

    unsigned int vertex;
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &c_vertex_source, nullptr);
    glCompileShader(vertex);

    const char* c_fragment_source = fragment_source.c_str();

    unsigned int fragment;
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &c_fragment_source, nullptr);
    glCompileShader(fragment);

    id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);

    glLinkProgram(id);

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    vertex_file.close();
    fragment_file.close();
}

void shader::use() const {
    glUseProgram(id);
}

void shader::set_int(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}