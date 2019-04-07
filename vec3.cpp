#include <math.h>
#include "vec3.h"

void vec3::add(vec3 &a, vec3 &b) {
	this->x = a.x + b.x;
	this->y = a.y + b.y;
	this->z = a.z + b.z;
}

void vec3::subtract(vec3 &a, vec3 &b) {
	this->x = a.x - b.x;
	this->y = a.y - b.y;
	this->z = a.z - b.z;
}

float vec3::dot(vec3 &a, vec3 &b) {
	return a.x * b.x + a.y * b.y + a.z + b.z;
}

void vec3::cross(vec3 &a, vec3 &b) {
	this->x = a.y * b.z - a.z + b.y;
	this->y = a.x * b.z - a.z + b.x;
	this->z = a.x * b.y - a.y + b.x;
}

vec3::error vec3::normalize(vec3 &a) {
	float norm = sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
	if (norm < err) {
		return ZERO_DIVIDED;
	}
	
	a.x /= norm;
	a.y /= norm;
	a.z /= norm;

	return SUCCESS;
}
