//
// Created by mykola on 10.05.24.
//

#ifndef BVH_BVH_H
#define BVH_BVH_H

#include <vector>
#include <limits>
#include <algorithm>

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

    aabb bounding_box() const {
        return box;
    }

    vector3 get_centroid() const {
        return centroid;
    }
};


class bvh {
    std::vector<bvh_primitive> primitives;
    aabb box;

public:
    explicit bvh(const std::vector<triangle>& triangles) {
        for (const auto& tri : triangles) {
            primitives.emplace_back(tri);
        }

        box = aabb{vector3{std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max()},
                   vector3{std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min()}};

        for (const auto& prim : primitives) {
            box.min_corner.data[0] = std::min(box.min_corner.data[0], prim.bounding_box().min_corner.data[0]);
            box.min_corner.data[1] = std::min(box.min_corner.data[1], prim.bounding_box().min_corner.data[1]);
            box.min_corner.data[2] = std::min(box.min_corner.data[2], prim.bounding_box().min_corner.data[2]);

            box.max_corner.data[0] = std::max(box.max_corner.data[0], prim.bounding_box().max_corner.data[0]);
            box.max_corner.data[1] = std::max(box.max_corner.data[1], prim.bounding_box().max_corner.data[1]);
            box.max_corner.data[2] = std::max(box.max_corner.data[2], prim.bounding_box().max_corner.data[2]);
        }
    }
};

#endif //BVH_BVH_H
