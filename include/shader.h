//
// Created by mykola on 08.04.24.
//

#ifndef PATH_TRACING_SHADER_H
#define PATH_TRACING_SHADER_H

#include <string>

class shader {
public:
    unsigned int id;
    shader(const std::string& vertex_path, const std::string& fragment_path);

    void use() const;

    void set_int(const std::string& name, int value) const;
};

#endif //PATH_TRACING_SHADER_H
