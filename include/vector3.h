//
// Created by mykola on 11.05.24.
//

#ifndef BVH_VECTOR3_H
#define BVH_VECTOR3_H

class vector3 {
public:
    float data[3];

public:
    vector3(): data{0, 0, 0} {}
    vector3(float x, float y, float z): data{x, y, z} {}

    vector3 operator+(const vector3& other) const {
        return vector3{data[0] + other.data[0], data[1] + other.data[1], data[2] + other.data[2]};
    }

    vector3 operator-(const vector3& other) const {
        return vector3{data[0] - other.data[0], data[1] - other.data[1], data[2] - other.data[2]};
    }

    vector3 operator*(float scalar) const {
        return vector3{data[0] * scalar, data[1] * scalar, data[2] * scalar};
    }
};

#endif //BVH_VECTOR3_H
