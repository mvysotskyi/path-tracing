//
// Created by mykola on 10.05.24.
//

#ifndef BVH_BVH_H
#define BVH_BVH_H

#include <iostream>
#include <stack>
#include <vector>
#include <limits>
#include <algorithm>
#include <fstream>

#include "aabb.h"
#include "triangle.h"

class bvh_primitive {
    triangle tri;
    aabb box;
    vector3 centroid;

public:
    explicit bvh_primitive(const triangle& tri): tri(tri) {
        box = aabb::from_triangle(tri);
        centroid = box.min_corner * 0.5f + box.max_corner * 0.5f;
    }

    const aabb& bounding_box() const {
        return box;
    }

    vector3 get_centroid() const {
        return centroid;
    }

    const triangle& get_triangle() const {
        return tri;
    }
};

struct bvh_node {
    aabb box;
    bvh_node* left;
    bvh_node* right;
    bvh_primitive* primitive;
};

class bvh {
    std::vector<bvh_primitive> m_primitives;
    bvh_node* root;

public:
    explicit bvh(const std::vector<triangle>& triangles) {
        for (const auto& tri : triangles) {
            m_primitives.emplace_back(tri);
        }
    }

    void build() {
        root = build_recursive(m_primitives, 0, m_primitives.size());
    }

    const bvh_node* get() const {
        if(root == nullptr)
            throw std::runtime_error("BVH not built");
        return root;
    }

    void print() {
        std::stack<bvh_node*> stack;
        stack.push(root);

        while (!stack.empty()) {
            bvh_node* node = stack.top();
            stack.pop();

            std::cout << "Box: (" << node->box.min_corner.data[0] << ", " << node->box.min_corner.data[1] << ", " << node->box.min_corner.data[2] << ") - ("
                      << node->box.max_corner.data[0] << ", " << node->box.max_corner.data[1] << ", " << node->box.max_corner.data[2] << ")" << std::endl;

            if (node->left) {
                stack.push(node->left);
            }

            if (node->right) {
                stack.push(node->right);
            }
        }
    }

private:
    bvh_node* build_recursive(std::vector<bvh_primitive>& primitives, size_t start, size_t end) {
        if (start == end) {
            return nullptr;
        }

        bvh_node* node = new bvh_node;
        node->box = aabb();
        for (size_t i = start; i < end; i++) {
            node->box = aabb::surrounding_box(node->box, primitives[i].bounding_box());
        }

        if (end - start == 1) {
            node->left = nullptr;
            node->right = nullptr;
            node->primitive = &primitives[start];
        } else {
            size_t axis = node->box.longest_axis();
            size_t mid = (start + end) / 2;
            std::nth_element(primitives.begin() + start, primitives.begin() + mid, primitives.begin() + end,
                             [axis](const bvh_primitive& a, const bvh_primitive& b) {
                                 return a.get_centroid().data[axis] < b.get_centroid().data[axis];
                             });

            node->left = build_recursive(primitives, start, mid);
            node->right = build_recursive(primitives, mid, end);
            node->primitive = nullptr;
        }

        return node;
    }

public:
    std::vector<aabb> serialize() {
        std::vector<aabb> boxes;
        std::stack<bvh_node*> stack;
        stack.push(root);

        while (!stack.empty()) {
            bvh_node* node = stack.top();
            stack.pop();

            boxes.push_back(node->box);

            if (node->left) {
                stack.push(node->left);
            }

            if (node->right) {
                stack.push(node->right);
            }
        }

        return boxes;
    }

    // Save a list of AABB boxes to an OBJ file
    static void aabb_boxes_save_obj(const std::vector<aabb>& boxes, const std::string& filename) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file");
        }

        for (const auto& box : boxes) {
            file << "v " << box.min_corner.data[0] << " " << box.min_corner.data[1] << " " << box.min_corner.data[2] << std::endl;
            file << "v " << box.min_corner.data[0] << " " << box.min_corner.data[1] << " " << box.max_corner.data[2] << std::endl;
            file << "v " << box.min_corner.data[0] << " " << box.max_corner.data[1] << " " << box.min_corner.data[2] << std::endl;
            file << "v " << box.min_corner.data[0] << " " << box.max_corner.data[1] << " " << box.max_corner.data[2] << std::endl;
            file << "v " << box.max_corner.data[0] << " " << box.min_corner.data[1] << " " << box.min_corner.data[2] << std::endl;
            file << "v " << box.max_corner.data[0] << " " << box.min_corner.data[1] << " " << box.max_corner.data[2] << std::endl;
            file << "v " << box.max_corner.data[0] << " " << box.max_corner.data[1] << " " << box.min_corner.data[2] << std::endl;
            file << "v " << box.max_corner.data[0] << " " << box.max_corner.data[1] << " " << box.max_corner.data[2] << std::endl;
        }

        // Lines
        for (size_t i = 0; i < boxes.size(); i++) {
            file << "l " << i * 8 + 1 << " " << i * 8 + 2 << std::endl;
            file << "l " << i * 8 + 1 << " " << i * 8 + 3 << std::endl;
            file << "l " << i * 8 + 1 << " " << i * 8 + 5 << std::endl;
            file << "l " << i * 8 + 2 << " " << i * 8 + 4 << std::endl;
            file << "l " << i * 8 + 2 << " " << i * 8 + 6 << std::endl;
            file << "l " << i * 8 + 3 << " " << i * 8 + 4 << std::endl;
            file << "l " << i * 8 + 3 << " " << i * 8 + 7 << std::endl;
            file << "l " << i * 8 + 4 << " " << i * 8 + 8 << std::endl;
            file << "l " << i * 8 + 5 << " " << i * 8 + 6 << std::endl;
            file << "l " << i * 8 + 5 << " " << i * 8 + 7 << std::endl;
            file << "l " << i * 8 + 6 << " " << i * 8 + 8 << std::endl;
            file << "l " << i * 8 + 7 << " " << i * 8 + 8 << std::endl;
        }

        file.close();
    }
};

#endif //BVH_BVH_H
