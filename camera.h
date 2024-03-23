#pragma once

#include "defines.h"
#include "vec/vec.h"
#include "entity.h"

#include <Windows.h>    
#include <gl/GL.h>    
#include <gl/GLU.h>

class Camera {
public:
	const vec3f y_vec_p1 = vec3f(0.0f, 1.0f, 0.0f);
	const vec3f z_vec_m1 = vec3f(0.0f, 0.0f, -1.0f);

	const float fov_y = 60.0f;
	const float va_max = (M_PI / 180.0f) * 80.0f;

	vec3f static_position;
	vec3f* position;
	vec3f target;

	float* direct_ang;
	float* vertical_ang;

	unsigned __int64* upd_check = nullptr;

private:


public:
	Camera(const vec3f& initial_position);
	void bind_to(Entity* entity);
	void update();
};