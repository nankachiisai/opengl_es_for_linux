#ifndef _VEC3_H_
#define _VEC3_H_

class vec3 {
public:
	float x, y, z;

	typedef enum {
		SUCCESS = 0,
		ZERO_DIVIDED = 1
	} error;

	void add(vec3 &a, vec3 &b);
	void subtract(vec3 &a, vec3 &b);
	float dot(vec3 &a, vec3 &b);
	void cross(vec3 &a, vec3 &b);
	error normalize(vec3 &a);

private:
	const float err = 0.01;
};

#endif