#pragma once

#include "entity.h"
#include "camera.h"

class Player: public Entity {
private:
	friend class Camera;

public:
	const char moving_keys[4] = {'W', 'S', 'D', 'A'};

	float moving_speed = 0.04f;
	float rotate_speed = 0.002f;

	bool moving_state[8] = {
		false, false, false, false, // Moving states
		false,					    // Rotation state 
		0, 0, 0					    // garbage 
	};

public:
	Player(const vec3f& position);
	~Player();

	void update();
	void move();
	void mouse_position_update(double dx, double dy);
	int keydown_update(KEY key);
	int keyup_update(KEY key);
};