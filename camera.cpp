#include "camera.h"

#include <iostream>

Camera::Camera(const vec3f& initial_position) {
	static_position = initial_position;
	position = &static_position;

	this->target = static_position + z_vec_m1;

	direct_ang = nullptr;
	vertical_ang = nullptr;
}

void Camera::bind_to(Entity* entity) {
	position = &(entity->_position);
	direct_ang = &(entity->horizontal_direction);
	vertical_ang = &(entity->vertical_direction);
}

void Camera::update() {
	target = z_vec_m1;

	if (direct_ang) {
		target.x = sinf(*direct_ang);
		target.z = cosf(*direct_ang);
	}

	if (vertical_ang) {
		
	}

	target += *position;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		COORDS3P(position),
		COORDS3(target),
		COORDS3(y_vec_p1)
	);
}