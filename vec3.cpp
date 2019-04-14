#include <math.h>
#include "vec3.h"

vec3::vec3() {
}

vec3::vec3(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

vec3::vec3(vec3 &v) {
	this->x = v.x;
	this->y = v.y;
	this->z = v.z;
}

void vec3::add(const vec3 &a, const vec3 &b) {
	this->x = a.x + b.x;
	this->y = a.y + b.y;
	this->z = a.z + b.z;
}

void vec3::subtract(const vec3 &a, const vec3 &b) {
	this->x = a.x - b.x;
	this->y = a.y - b.y;
	this->z = a.z - b.z;
}

void vec3::multiply(float a) {
	this->x *= a;
	this->y *= a;
	this->z *= a;
}

void vec3::cross(const vec3 &a, const vec3 &b) {
	this->x = a.y * b.z - a.z * b.y;
	this->y = a.x * b.z - a.z * b.x;
	this->z = a.x * b.y - a.y * b.x;
}

vec3::error vec3::normalize() {
	float norm = sqrtf(this->x * this->x + this->y * this->y + this->z * this->z);
	if (norm < err) {
		return ZERO_DIVIDE;
	}
	
	this->x /= norm;
	this->y /= norm;
	this->z /= norm;

	return SUCCESS;
}

float vec3::dot(const vec3 &a, const vec3 &b) {
	return a.x * b.x + a.y * b.y + a.z + b.z;
}