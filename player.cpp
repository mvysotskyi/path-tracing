#include "player.h"

#include <cstring>
#include <iostream>

Player::Player(const vec3f& position) {
	this->_position = position;
	upd_check = (unsigned __int64*)moving_state;
}

Player::~Player() {
	
}

void Player::move() {
	
}

void Player::update() {
	if (*upd_check) {
		for (int i = 0; i < 4; ++i) {
			if (moving_state[i]) {
				float sin_ha = sin(RADIANS(_h_angle));
				float cos_ha = cos(RADIANS(_h_angle));

				switch(moving_keys[i]) {
				case 'W':
					_position += vec3f(sin_ha * moving_speed, 0 , cos_ha * moving_speed);
					break;
				case 'S':
					_position += vec3f(-sin_ha * moving_speed, 0, -cos_ha * moving_speed);
					break;
				default:
					break;
				}
			}
		}
	}
}

void Player::mouse_position_update(double dx, double dy) {
	if (dx) {
		_h_angle -= (float)dx * rotate_speed;
	}

	if (dy) {
		float va_check = _v_angle - (float)dy * rotate_speed;

		if (va_check <= M_PI && va_check >= -M_PI) {
			_v_angle = va_check;
		}
	}
}

int Player::keydown_update(KEY key) {
	const char* pos = (char*)strchr(moving_keys, key);
	if (pos != NULL) {
		moving_state[pos - moving_keys] = true;
	}

	return 0;
}

int Player::keyup_update(KEY key) {
	const char* pos = (char*)strchr(moving_keys, key);
	if (pos != NULL) {
		moving_state[pos - moving_keys] = false;
	}

	return 0;
}