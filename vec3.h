#ifndef _VEC3_H_
#define _VEC3_H_

class vec3 {
public:
	float x, y, z;

	typedef enum {
		SUCCESS = 0,
		ZERO_DIVIDE = 1
	} error;

	vec3();
	vec3(float x, float y, float z);
	vec3(vec3 &v);
	void add(const vec3 &a, const vec3 &b);
	void subtract(const vec3 &a, const vec3 &b);
	void multiply(float a);
	void cross(const vec3 &a, const vec3 &b);
	error normalize();

	static float dot(const vec3 &a, const vec3 &b);

private:
	const float err = 0.001f;
};

#endif