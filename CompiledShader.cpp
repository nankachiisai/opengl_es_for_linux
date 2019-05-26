#include "CompiledShader.h"

CompiledShader::CompiledShader(GLuint ShaderType, GLuint CompiledShaderNumber) {
	this->ShaderType = ShaderType;
	this->CompiledShaderNumber = CompiledShaderNumber;
}

CompiledShader::~CompiledShader() {
	// シェーダオブジェクトを削除する
	glDeleteShader(this->CompiledShaderNumber);
}

GLuint CompiledShader::getCompiledShaderNumber() {
	return this->CompiledShaderNumber;
}