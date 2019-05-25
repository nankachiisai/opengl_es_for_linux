#include <fstream>
#include "ShaderSource.h"
#include "Exception.h"

using namespace std;

ShaderSource::ShaderSource(const std::string FileName, const GLuint ShaderType) {
	this->ShaderType = ShaderType;

	// ファイルオープン
	string filename_str(FileName);
	ifstream f(filename_str, ios::in | ios::binary);
	if (f.fail()) {
		throw Exception("error: ifstream constructor\n");
	}
	
	// とりあえず1KB確保
	int strsize = 1024;
	string *source = new string(strsize, '\0');

	// 終端までファイルを読み取る
	int count = 0;
	while (1) {
		int readNum = strsize - count;
		f.read(&(*source)[count], readNum);
		if (f.eof()) {
			break;
		}

		count = strsize;
		strsize *= 2;
		source->resize(strsize, 0);
	}

	// ソース文字列を格納する
	this->SourceString = source;
}

ShaderSource::~ShaderSource() {
	delete this->SourceString;
}

CompiledShader *ShaderSource::compile() {
	// シェーダオブジェクトを作成する
	GLuint shader = glCreateShader(this->ShaderType);

	// コンパイル
	const char *src = &(this->SourceString->operator[](0));
	const int size = this->SourceString->size();
	glShaderSource(shader, 1, &src, &size);
	glCompileShader(shader);

	// コンパイルステータスをgetする
	GLint compileStatus;
	const int logSize = 1024;
	string Log(logSize, '\0');
	GLsizei length;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus == GL_FALSE) {
		glGetShaderInfoLog(shader, logSize * sizeof(GLchar), &length, &Log[0]);
		string errorLog("Error; glGetShaderiv\n" + Log + "\n");
		throw Exception(errorLog);
		return NULL;
	}

	// コンパイル済みシェーダオブジェクトを返す
	CompiledShader *c = new CompiledShader(this->ShaderType, shader);
	return c;
}