#ifndef _MAT4_H_
#define _MAT4_H_

class mat4 {
public:
	float matrix[16];

	mat4();
	mat4(float m[16]);
	mat4(mat4 &m);
	void add(mat4 &a, mat4 &b);
	void subtract(mat4 &a, mat4 &b);
	void multiply(mat4 &a, mat4 &b);
	void multiply(float a);
	void identity();
	void inverse();
	void transpose();
};

#endif