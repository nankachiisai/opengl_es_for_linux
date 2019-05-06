#ifndef _VEC3_H_
#define _VEC3_H_

#include <math.h>

class vec3 {
public:
	float x, y, z;

	typedef enum {
		SUCCESS = 0,
		ZERO_DIVIDE = 1
	} error;

	vec3() {
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

	vec3(float x, float y, float z) {
		vec3();

		this->x = x;
		this->y = y;
		this->z = z;
	}

	vec3(vec3 &v) {
		vec3();

		this->x = v.x;
		this->y = v.y;
		this->z = v.z;
	}

	void add(const vec3 &a, const vec3 &b) {
		this->x = a.x + b.x;
		this->y = a.y + b.y;
		this->z = a.z + b.z;
	}

	void subtract(const vec3 &a, const vec3 &b) {
		this->x = a.x - b.x;
		this->y = a.y - b.y;
		this->z = a.z - b.z;
	}

	void multiply(float a) {
		this->x *= a;
		this->y *= a;
		this->z *= a;
	}

	void cross(const vec3 &a, const vec3 &b) {
		this->x = a.y * b.z - a.z * b.y;
		this->y = a.x * b.z - a.z * b.x;
		this->z = a.x * b.y - a.y * b.x;
	}

	error normalize() {
		float norm = sqrtf(this->x * this->x + this->y * this->y + this->z * this->z);
		if (norm < err) {
			return ZERO_DIVIDE;
		}

		this->x /= norm;
		this->y /= norm;
		this->z /= norm;

		return SUCCESS;
	}

	static float dot(const vec3 &a, const vec3 &b) {
		return a.x * b.x + a.y * b.y + a.z + b.z;
	}

private:
	float err = 1.0e-6f;
};

#endif