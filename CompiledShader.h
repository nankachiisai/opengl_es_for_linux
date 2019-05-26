#ifndef _COMPILEDSHADER_H_
#define _COMPILEDSHADER_H_

#include <GL/glew.h>
#include <GL/glut.h>

class CompiledShader {
private:
	GLuint ShaderType;
	GLuint CompiledShaderNumber;

public:
	CompiledShader(GLuint ShaderType, GLuint CompiledShaderNumber);
	CompiledShader() {}
	~CompiledShader();
	GLuint getCompiledShaderNumber();
};

class VertexShader : public CompiledShader {
public:
	VertexShader(GLuint CompiledShaderNumber) {
		CompiledShader::CompiledShader(GL_VERTEX_SHADER, CompiledShaderNumber);
	}

	~VertexShader() {
		CompiledShader::~CompiledShader();
	}
};

class FragmentShader: public CompiledShader {
public:
	FragmentShader(GLuint CompiledShaderNumber) {
		CompiledShader::CompiledShader(GL_FRAGMENT_SHADER, CompiledShaderNumber);
	}

	~FragmentShader() {
		CompiledShader::~CompiledShader();
	}
};

#endif