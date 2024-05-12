//
// Created by mykola on 11.05.24.
//

#ifndef BVH_TRIANGLE_H
#define BVH_TRIANGLE_H

#include <stdexcept>

#include "vector3.h"
#include "tiny_obj_loader.h"

#include <glm/glm.hpp>

struct shader_triangle {
    using vec3 = glm::vec3;
    vec3 vertices[3];
    vec3 normal;
    vec3 emission;
    vec3 color;
    unsigned int recflection_type;
};

class triangle {
    vector3 vertices[3];
    vector3 normal;

public:
    triangle(const tinyobj::shape_t& shape, const tinyobj::attrib_t& attrib, size_t index) {
        for (int i = 0; i < 3; i++) {
            size_t vertex_index = shape.mesh.indices[index + i].vertex_index;
            vertices[i] = vector3{attrib.vertices[3 * vertex_index], attrib.vertices[3 * vertex_index + 1], attrib.vertices[3 * vertex_index + 2]};
        }

        // Calculate normal
        vector3 edge1 = vertices[1] - vertices[0];
        vector3 edge2 = vertices[2] - vertices[0];
        normal = edge1.cross(edge2).normalize();
    }

    triangle(const vector3& v0, const vector3& v1, const vector3& v2): vertices{v0, v1, v2} {}

    const vector3& vertex(int i) const {
        if(i < 0 || i > 2) {
            throw std::out_of_range("Triangle vertex index out of range");
        }
        return vertices[i];
    }

    shader_triangle to_shader_triangle() const {
        shader_triangle tri{};
        for (int i = 0; i < 3; i++) {
            tri.vertices[i][0] = vertices[i].data[0];
            tri.vertices[i][1] = vertices[i].data[1];
            tri.vertices[i][2] = vertices[i].data[2];
        }
        tri.normal[0] = normal.data[0];
        tri.normal[1] = normal.data[1];
        tri.normal[2] = normal.data[2];

        tri.emission[0] = 0.0f;
        tri.emission[1] = 0.0f;
        tri.emission[2] = 0.0f;

        tri.color[0] = 0.5f;
        tri.color[1] = 0.5f;
        tri.color[2] = 0.5f;

        tri.recflection_type = 1u;

        return tri;
    }
};

#endif //BVH_TRIANGLE_H
