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
	virtual ~CompiledShader();
	GLuint getCompiledShaderNumber();
};

class VertexShader : public CompiledShader {
public:
	VertexShader(GLuint CompiledShaderNumber) : CompiledShader(GL_VERTEX_SHADER, CompiledShaderNumber) {
	}

	~VertexShader() {
	}
};

class FragmentShader: public CompiledShader {
public:
	FragmentShader(GLuint CompiledShaderNumber) : CompiledShader(GL_FRAGMENT_SHADER, CompiledShaderNumber) {
	}

	~FragmentShader() {
	}
};

#endif