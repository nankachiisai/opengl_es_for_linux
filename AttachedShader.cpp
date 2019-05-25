#include <string>
#include "AttachedShader.h"
#include "Exception.h"

using namespace std;

AttachedShader::AttachedShader(VertexShader &v, FragmentShader &f) {
	// シェーダプログラムを作成
	GLuint shaderp = glCreateProgram();

	// バーテックスシェーダとフラグメントシェーダをアタッチする
	glAttachShader(shaderp, v.getCompiledShaderNumber());
	glAttachShader(shaderp, f.getCompiledShaderNumber());

	// シェーダをリンクする
	glLinkProgram(shaderp);

	// リンクステータスをgetする
	GLint linkStatus;
	const int logSize = 1024;
	string log(logSize, '\0');
	GLsizei length;
	glGetProgramiv(shaderp, GL_LINK_STATUS, &linkStatus);
	if (linkStatus == GL_FALSE) {
		glGetProgramInfoLog(shaderp, logSize * sizeof(GLchar), &length, &log[0]);
		string errorLog("error: glGetProgramiv\n" + log);
		throw Exception(errorLog);
		return;
	}

	// シェーダプログラムを適用する
	glUseProgram(shaderp);
	this->ProgramNumber = shaderp;
}

AttachedShader::~AttachedShader() {
	// シェーダプログラムを削除する
	glDeleteProgram(this->ProgramNumber);
}

GLuint AttachedShader::getProgramNumber() {
	return this->ProgramNumber;
}