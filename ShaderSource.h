#ifndef _SHADERSOURCE_H_
#define _SHADERSOURCE_H_

#include <GL/glew.h>
#include <GL/glut.h>
#include <string>
#include "CompiledShader.h"

class ShaderSource {
private:
	std::string *SourceString;
	GLuint ShaderType;

public:
	ShaderSource(const std::string FileName, const GLuint ShaderType);
	~ShaderSource();
	CompiledShader *compile();
};

#endif