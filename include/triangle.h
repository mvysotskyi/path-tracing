//
// Created by mykola on 11.05.24.
//

#ifndef BVH_TRIANGLE_H
#define BVH_TRIANGLE_H

#include <stdexcept>

#include "vector3.h"
#include "tiny_obj_loader.h"

#include <glm/glm.hpp>

using vec3 = glm::vec3;

struct shader_triangle {
    vec3 vertices[3];
    vec3 normal;
    vec3 emission;
    vec3 color;
    int32_t recflection_type;
};

struct triangle {
    uint32_t vertices_ids[3];

public:
    triangle(const tinyobj::shape_t& shape, const tinyobj::attrib_t& attrib, size_t index) {
        for (size_t i = 0; i < 3; i++) {
            vertices_ids[i] = shape.mesh.indices[index + i].vertex_index;
        }
    }
};

#endif //BVH_TRIANGLE_H
