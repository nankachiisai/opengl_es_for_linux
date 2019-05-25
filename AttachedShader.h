#ifndef _ATTACHEDSHADER_H_
#define _ATTACHEDSHADER_H_

#include <GL/glew.h>
#include <GL/glut.h>
#include "CompiledShader.h"

class AttachedShader {
private:
	GLuint ProgramNumber;

public:
	AttachedShader(VertexShader &v, FragmentShader &f);
	~AttachedShader();
	GLuint getProgramNumber();
};

#endif