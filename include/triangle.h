//
// Created by mykola on 11.05.24.
//

#ifndef BVH_TRIANGLE_H
#define BVH_TRIANGLE_H

#include <stdexcept>

#include "vector3.h"
#include "tiny_obj_loader.h"

class triangle {
    vector3 vertices[3];
    vector3 normal;

public:
    triangle(const tinyobj::shape_t& shape, const tinyobj::attrib_t& attrib, size_t index) {
        for (int i = 0; i < 3; i++) {
            size_t vertex_index = shape.mesh.indices[index + i].vertex_index;
            vertices[i] = vector3{attrib.vertices[3 * vertex_index], attrib.vertices[3 * vertex_index + 1], attrib.vertices[3 * vertex_index + 2]};
        }

        // TODO: Calculate normal
    }

    const vector3& vertex(int i) const {
        if(i < 0 || i > 2) {
            throw std::out_of_range("Triangle vertex index out of range");
        }
        return vertices[i];
    }
};

#endif //BVH_TRIANGLE_H
