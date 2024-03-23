#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

#define ABS_MAX(X, Y) ((fabs(X) < fabs(Y)) ? (Y) : (X))
#define RADIANS(x) (x) * (180.0f / M_PI)

float q_rsqrt(float x);