//
// Created by mykola on 08.04.24.
//

#ifndef PATH_TRACING_COMPUTE_SHADER_H
#define PATH_TRACING_COMPUTE_SHADER_H

#include <string>

class compute_shader {
public:
    unsigned int id;
    compute_shader(const std::string& path);

    void use() const;

    void set_float(const std::string& name, float value) const;
    void set_int(const std::string& name, int value) const;

    void checkCompileErrors(GLuint shader, std::string type);
};

#endif //PATH_TRACING_COMPUTE_SHADER_H
