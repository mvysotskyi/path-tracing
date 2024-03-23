#pragma once

#include "defines.h"

class Camera;

class Entity {
protected:
	friend class Camera;

	vec3f _position;
	float horizontal_direction;
	float vertical_direction;

	unsigned __int64* upd_check = nullptr;

public:
	Entity();
	Entity(const vec3f& initial_position);
	~Entity();

	vec3f& position();
	virtual void update() = 0;
};