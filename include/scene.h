//
// Created by mykola on 10.05.24.
//

#ifndef BVH_SCENE_H
#define BVH_SCENE_H

#include <iostream>
#include <vector>
#include <limits>

#include "bvh.h"
#include "triangle.h"
#include "tiny_obj_loader.h"


class scene {
    std::vector<triangle> triangles;

public:
    explicit scene(const std::string& obj_filepath) {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, obj_filepath.c_str())) {
            throw std::runtime_error(warn + err);
        }

        for (const auto& shape : shapes) {
            for (size_t index = 0; index < shape.mesh.indices.size(); index += 3) {
                triangles.emplace_back(shape, attrib, index);
            }
        }
    }

public:
    void print_triangles() {
        for (const auto& triangle : triangles) {
            std::cout << "Triangle vertices: ";
            for (int i = 0; i < 3; i++) {
                std::cout << "(" << triangle.vertex(i).data[0] << ", " << triangle.vertex(i).data[1] << ", " << triangle.vertex(i).data[2] << ")";
            }
            std::cout << std::endl;
        }
    }

    std::vector<float> vertices() const {
        std::vector<float> vertices;
        for (const auto& triangle : triangles) {
            for (int i = 0; i < 3; i++) {
                vertices.push_back(triangle.vertex(i).data[0]);
                vertices.push_back(triangle.vertex(i).data[1]);
                vertices.push_back(triangle.vertex(i).data[2]);
            }
        }
        return vertices;
    }

    std::vector<unsigned int> indices() const {
        std::vector<unsigned int> indices;
        for (size_t i = 0; i < triangles.size(); i++) {
            indices.push_back(i * 3);
            indices.push_back(i * 3 + 1);
            indices.push_back(i * 3 + 2);
        }
        return indices;
    }

    bvh get_bvh() {
        return bvh(triangles);
    }

    const std::vector<triangle>& get_triangles() const {
        return triangles;
    }
};

#endif //BVH_SCENE_H
