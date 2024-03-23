#include "entity.h"

Entity::Entity() {
	_position = vec3f(0.0f, 0.0f, 0.0f);
	_h_angle = 0.0f;
}

Entity::Entity(const vec3f& initial_position) {
	_position = initial_position;
	_h_angle = 0.0f;
}

Entity::~Entity() {

}

vec3f& Entity::position() {
	return _position;
}