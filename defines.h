#pragma once

#define COORDS3(vec) vec.x, vec.y, vec.z
#define COORDS3P(pvec) pvec->x, pvec->y, pvec->z
#define COORDS2(vec) vec.x, vec.y
#define COORDS2P(pvec) pvec->x, pvec->y

#include "fmath/fmath.h"
#include "glm/GLM/glm.hpp"

typedef glm::vec<3, float> vec3f;
typedef glm::vec<2, float> vec2f;
typedef glm::vec<3, unsigned long> vec3ul;

typedef int KEY;
typedef void(__stdcall *F_REDISPLAY)(void); // unused