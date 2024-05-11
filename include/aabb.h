//
// Created by mykola on 10.05.24.
//

#ifndef BVH_AABB_H
#define BVH_AABB_H

#include <algorithm>

#include "triangle.h"

class aabb {
public:
    vector3 min_corner;
    vector3 max_corner;

public:
    aabb(): min_corner(), max_corner() {}
    aabb(const vector3& min_corner, const vector3& max_corner): min_corner(min_corner), max_corner(max_corner) {}

    int longest_axis() const {
        vector3 box_size = max_corner - min_corner;
        if (box_size.data[0] > box_size.data[1] && box_size.data[0] > box_size.data[2]) {
            return 0;
        } else if (box_size.data[1] > box_size.data[2]) {
            return 1;
        } else {
            return 2;
        }
    }

    static aabb from_triangle(const triangle& tri) {
        vector3 min_corner{std::min(tri.vertex(0).data[0], std::min(tri.vertex(1).data[0], tri.vertex(2).data[0])),
                           std::min(tri.vertex(0).data[1], std::min(tri.vertex(1).data[1], tri.vertex(2).data[1])),
                           std::min(tri.vertex(0).data[2], std::min(tri.vertex(1).data[2], tri.vertex(2).data[2]))};
        vector3 max_corner{std::max(tri.vertex(0).data[0], std::max(tri.vertex(1).data[0], tri.vertex(2).data[0])),
                           std::max(tri.vertex(0).data[1], std::max(tri.vertex(1).data[1], tri.vertex(2).data[1])),
                           std::max(tri.vertex(0).data[2], std::max(tri.vertex(1).data[2], tri.vertex(2).data[2]))};
        return aabb{min_corner, max_corner};
    }

    static aabb surrounding_box(const aabb& box0, const aabb& box1) {
        vector3 min_corner{std::min(box0.min_corner.data[0], box1.min_corner.data[0]),
                           std::min(box0.min_corner.data[1], box1.min_corner.data[1]),
                           std::min(box0.min_corner.data[2], box1.min_corner.data[2])};
        vector3 max_corner{std::max(box0.max_corner.data[0], box1.max_corner.data[0]),
                           std::max(box0.max_corner.data[1], box1.max_corner.data[1]),
                           std::max(box0.max_corner.data[2], box1.max_corner.data[2])};
        return aabb{min_corner, max_corner};
    }
};


//struct ray {
//    std::array<float, 3> origin;
//    std::array<float, 3> direction;
//};
//
//struct aabb {
//    std::array<float, 3> min_corner;
//    std::array<float, 3> max_corner;
//
//    bool hit(const ray& r, float t_min, float t_max) const {
//        for (int a = 0; a < 3; a++) {
//            float inv_dir = 1.0f / r.direction[a];
//            float t0 = (min_corner[a] - r.origin[a]) * inv_dir;
//            float t1 = (max_corner[a] - r.origin[a]) * inv_dir;
//
//            if (inv_dir < 0.0f) {
//                std::swap(t0, t1);
//            }
//
//            t_min = t0 > t_min ? t0 : t_min;
//            t_max = t1 < t_max ? t1 : t_max;
//
//            if (t_max <= t_min) {
//                return false;
//            }
//        }
//        return true;
//    }
//};

#endif //BVH_AABB_H
