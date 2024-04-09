//
// Created by mykola on 08.04.24.
//

#include <fstream>
#include <iostream>
#include <GL/glew.h>

#include "compute_shader.h"

compute_shader::compute_shader(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << path << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string source;
    std::string line;

    while (std::getline(file, line)) {
        source += line + '\n';
    }

    const char* c_source = source.c_str();

    unsigned int compute;
    compute = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(compute, 1, &c_source, nullptr);
    glCompileShader(compute);
    checkCompileErrors(compute, "COMPUTE");

    id = glCreateProgram();
    glAttachShader(id, compute);
    glLinkProgram(id);
    checkCompileErrors(id, "PROGRAM");

    glDeleteShader(compute);

    file.close();
}

void compute_shader::use() const {
    glUseProgram(id);
}

void compute_shader::set_float(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void compute_shader::set_int(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void compute_shader::checkCompileErrors(GLuint shader, std::string type)
{
    GLint success;
    GLchar infoLog[1024];
    if(type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}