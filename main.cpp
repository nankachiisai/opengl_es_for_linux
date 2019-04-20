// for Windows
#define _CRT_SECURE_NO_WARNINGS

#include <GL/glew.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "bunny.h"
#include "vec3.h"
#include "mat4.h"

using namespace std;

static void display(void);
static void idle(void);
static int getShaderSource(const char *fileName, GLenum shaderType, GLuint *compiledProgram);
static int useShaders(GLuint VertShader, GLuint FragShader, GLuint *program);
static int freeShaders(GLuint VertShader, GLuint FragShader, GLuint program);
static int transferData(void *data, int num, GLenum bufferType, GLuint *buffer);
static int bindAttributeVariable(GLuint program, GLuint VBO, const char *name);
static int bindUniformVariable4x4(GLuint program, float *data, const char *name);
static int loadBunny(const char *filename, bunny *b);
static int freeBunny(bunny *b);
static void createOrthogonal(float Left, float Right, float Top, float Bottom, float Near, float Far, mat4 &m);
static void createLookAt(vec3 &position, vec3 &orientation, vec3 &up, mat4 &m);
static int split(string str, const char splitChar, vector<string> **splitedStr);

const float PI = 3.14159f;

static float rotationMatrix[] = {
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
};

static float expantionMatrix[] = {
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
};

static bunny b;

static GLuint program;
GLuint VBO; // Vertex Buffer Object
GLuint IBO; // Index Buffer Object
GLuint normalVector;

int main(int argc, char *argv[]) {
	GLenum err;
	GLuint vShader, fShader;
	int returnValue;

	// スタンフォードバニーの読み込み
	returnValue = loadBunny("bun_zipper.ply", &b);
	if (returnValue == -1) {
		return -1;
	}

	// GLUTの初期化
	glutInit(&argc, argv);

	// ウィンドウの作成
	glutCreateWindow(argv[0]);

	// glewの初期化
	err = glewInit();
	if (err != GLEW_OK) {
		return -1;
	}

	// 描画関数の設定
	glutDisplayFunc(display);

	// アイドル関数の設定
	glutIdleFunc(idle);

	// シェーダを読み込む
	returnValue = getShaderSource("main.vert", GL_VERTEX_SHADER, &vShader);
	if (returnValue == -1) {
		printf("Error: getShaderSource\n");
		return -1;
	}

	getShaderSource("main.frag", GL_FRAGMENT_SHADER, &fShader);
	if (returnValue == -1) {
		printf("Error: getShaderSource\n");
		return -1;
	}

	useShaders(vShader, fShader, &program);
	if (returnValue == -1) {
		printf("Error: useShaders");
		return -1;
	}

	// データをGPUに転送する
	transferData(b.vertices, b.vertexNum, GL_ARRAY_BUFFER, &VBO);
	transferData(b.normalVectors, b.vectorNum, GL_ARRAY_BUFFER, &normalVector);
	transferData(b.vertexIndices, b.indexNum, GL_ELEMENT_ARRAY_BUFFER, &IBO);

	// VBOとバーテックスシェーダのin変数とを関連付ける
	bindAttributeVariable(program, VBO, "position");
	bindAttributeVariable(program, normalVector, "normal");

	// メインループ
	glutMainLoop();

	// シェーダオブジェクト、シェーダプログラムを破棄する
	freeShaders(vShader, fShader, program);
	program = 0;

	// バッファを削除する
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &IBO);

	// スタンフォードバニーを削除する
	freeBunny(&b);

	return 0;
}

static void display(void) {
	static float degree = 0.0;

	// ウィンドウを白で塗りつぶす
	GLfloat white[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glClearBufferfv(GL_COLOR, 0, white);

	// 平行投影変換行列を生成する
	mat4 orthogonalMatrix;
	createOrthogonal(-0.25f, 0.25f, 0.25f, -0.25f, 0.0f, 10.0f, orthogonalMatrix);
	
	// 視野変換行列を生成する
	vec3 position(0.0f, 0.0f, 0.0f);
	vec3 orientation(0.0f, 0.0f, -1.0f);
	vec3 up(0.0f, 1.0f, 0.0f);
	mat4 lookAtMatrix;
	createLookAt(position, orientation, up, lookAtMatrix);

	// 回転行列を生成する
	float rad = degree * PI / 180.0f;
	rotationMatrix[0] = cosf(rad);
	rotationMatrix[2] = sinf(rad);
	rotationMatrix[8] = -sinf(rad);
	rotationMatrix[10] = cosf(rad);

	// 変換行列をuniform変数に関連付ける
	mat4 rotationMatrix(::rotationMatrix);
	mat4 expantionMatrix(::expantionMatrix);
	mat4 transformMatrix, tmpMatrix, resultMatrix;
	transformMatrix.multiply(rotationMatrix, expantionMatrix);
	tmpMatrix.multiply(lookAtMatrix, orthogonalMatrix);
	resultMatrix.multiply(tmpMatrix, transformMatrix);
	bindUniformVariable4x4(program, resultMatrix.matrix, "transformMatrix");

	// 変換行列の逆行列を生成する
	mat4 invMatrix(resultMatrix);
	invMatrix.inverse();

	// 逆行列をuniform変数に関連付ける
	bindUniformVariable4x4(program , invMatrix.matrix, "invMatrix");

	// 描画
	glDrawElements(GL_TRIANGLES, b.indexNum, GL_UNSIGNED_INT, 0);

	glFlush();

	if (abs(degree - 360.0f) < 0.01f) {
		degree = 0.0f;
	}
	else {
		degree += 0.01f;
	}

	return;
}

void idle(void) {
	glutPostRedisplay();
}

static int getShaderSource(const char *fileName, GLenum shaderType, GLuint *compiledProgram) {
	// とりあえず1KB確保
	int capacity = 1024;
	int usage = 0;
	char *source = (char *) malloc(sizeof(char) * capacity);
	if (source == NULL) {
		printf("Error: malloc\n");
		return -1;
	}

	// ファイルオープン
	FILE *fp = fopen(fileName, "r");
	if (fp == NULL) {
		printf("Error: fopen\n");
		return -1;
	}

	// 終端までファイルを読み取る
	while (1) {
		int num;

		// バッファが満杯であれば、realloc
		if (capacity == usage) {
			char *tmpp;

			tmpp = (char *) realloc(source, capacity * 2);
			if (tmpp == NULL) {
				printf("Error: realloc\n");
				return -1;
			}
			source = tmpp;
			capacity *= 2;
		}

		// 文字列読み込み
		num = fread(&source[usage], sizeof(char), capacity - usage, fp);
		usage += num;

		// 読み取れるものがなくなったら終了
		if (num == 0) {
			// 念のため、Null terminateしておく
			source[usage] = '\0';
			usage++;

			break;
		}
	}

	// ファイルをクローズ
	int returnValue = fclose(fp);
	if (returnValue == EOF) {
		printf("Error: fclose\n");
		return -1;
	}
	fp = NULL;

	// シェーダオブジェクトを作成する
	GLuint shader = glCreateShader(shaderType);

	// コンパイル
	const char* src = source;
	glShaderSource(shader, 1, &src, &usage);
	glCompileShader(shader);

	// コンパイルステータスをgetする
	GLint compileStatus;
	GLchar Log[1024];
	GLsizei length;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus == GL_FALSE) {
		glGetShaderInfoLog(shader, 1024 * sizeof(GLchar), &length, Log);
		printf("Error: glGetShaderiv\n%s\n", Log);
		return -1;
	}

	// freeする
	free(source);
	source = NULL;

	// コンパイル済みシェーダオブジェクトを返す
	*compiledProgram = shader;

	return 0;
}

static int useShaders(GLuint VertShader, GLuint FragShader, GLuint *program) {
	// シェーダプログラムを作成
	GLuint shaderp = glCreateProgram();

	// バーテックスシェーダとフラグメントシェーダをアタッチする
	glAttachShader(shaderp, VertShader);
	glAttachShader(shaderp, FragShader);

	// シェーダをリンクする
	glLinkProgram(shaderp);

	// リンクステータスをgetする
	GLint linkStatus;
	GLchar Log[1024];
	GLsizei length;
	glGetProgramiv(shaderp, GL_LINK_STATUS, &linkStatus);
	if (linkStatus == GL_FALSE) {
		glGetProgramInfoLog(shaderp, 1024 * sizeof(GLchar), &length, Log);
		printf("Error: glGetProgramiv\n%s\n", Log);
		return -1;
	}

	// シェーダプログラムを適用する
	glUseProgram(shaderp);

	// シェーダプログラムを返す
	*program = shaderp;

	return 0;
}

static int freeShaders(GLuint VertShader, GLuint FragShader, GLuint program) {
	// シェーダオブジェクトを削除する
	glDeleteShader(VertShader);
	glDeleteShader(FragShader);

	// シェーダプログラムを削除する
	glDeleteProgram(program);

	return 0;
}

static int transferData(void *data, int num, GLenum bufferType, GLuint *buffer) {
	// バッファを作成する
	glGenBuffers(1, buffer);

	// バッファにデータをセットする
	glBindBuffer(bufferType, *buffer);
	glBufferData(bufferType, sizeof(float) * num, data, GL_STATIC_DRAW);

	return 0;
}

static int bindAttributeVariable(GLuint program, GLuint VBO, const char *name) {
	// VBOをバインドする
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// VBOとin変数を関連付ける
	GLint attr = glGetAttribLocation(program, name);
	glEnableVertexAttribArray(attr);
	glVertexAttribPointer(attr, 3, GL_FLOAT, GL_FALSE, 0, 0);

	return 0;
}

static int bindUniformVariable4x4(GLuint program, float *data, const char *name) {
	// uniform変数の場所をgetする
	GLint uniform = glGetUniformLocation(program, name);

	// 4x4の行列をuniform変数に関連付ける
	glUniformMatrix4fv(uniform, 1, GL_FALSE, data);

	return 0;
}

static int loadBunny(const char *filename, bunny *b) {
	int vertNum = 0;
	int idxNum = 0;

	//ファイルオープン
	ifstream file(filename);
	if (file.fail()) {
		printf("error: ifstream constructor\n");
		return -1;
	}

	// ヘッダ部
	while (1) {
		// stringクラスを使う
		string str;

		// 行を読み込む
		getline(file, str);

		// 読み込んだ文字列をspiltする
		vector<string> *v = NULL;
		split(str, ' ', &v);

		// elementセクションを読む
		if ((*v)[0] == "element") {
			if ((*v)[1] == "vertex") {
				sscanf(&((*v)[2])[0], "%d", &vertNum);
			}
			if ((*v)[1] == "face") {
				sscanf(&((*v)[2])[0], "%d", &idxNum);
			}
		}

		// end_headerだったら、ループを抜ける
		if (str == "end_header") {
			break;
		}

		// vectorをdeleteする
		delete v;
	}

	// 頂点配列を確保する。35947 * 3 * 4 = 431,364バイト必要。
	// 使用後、delete[]すること。
	float *vertices = new float[vertNum * 3];
	if (vertices == NULL) {
		return -1;
	}

	// ひたすら読み込んでいく
	float x, y, z, confidence, intensity;
	for (int i = 0; i < vertNum; i++) {
		string str;
		getline(file, str);
		sscanf(&str[0], "%f %f %f %f %f", &x, &y, &z, &confidence, &intensity);
		vertices[3 * i + 0] = x;
		vertices[3 * i + 1] = y;
		vertices[3 * i + 2] = z;
	}

	// 頂点インデックス配列を確保する。69451 * 3 * 4 = 833,412バイト必要。
	// 使用後、delete[]すること。
	unsigned int *indices = new unsigned int[idxNum * 3];
	if (indices == NULL) {
		return -1;
	}

	// ひたすら読み込む
	int tmp, ix, iy, iz;
	for (int i = 0; i < idxNum; i++) {
		string str;
		getline(file, str);
		sscanf(&str[0], "%d %d %d %d", &tmp, &ix, &iy, &iz);
		indices[3 * i + 0] = ix;
		indices[3 * i + 1] = iy;
		indices[3 * i + 2] = iz;
	}

	// 面法線ベクトルを三角形から計算する
	vec3 *surfNormals = new vec3[idxNum];

	for (int i = 0; i < idxNum; i++) {
		vec3 point1, point2, point3;
		point1.x = vertices[indices[3 * i + 0] + 0];
		point1.y = vertices[indices[3 * i + 0] + 1];
		point1.z = vertices[indices[3 * i + 0] + 2];

		point2.x = vertices[indices[3 * i + 1] + 0];
		point2.y = vertices[indices[3 * i + 1] + 1];
		point2.z = vertices[indices[3 * i + 1] + 2];

		point3.x = vertices[indices[3 * i + 2] + 0];
		point3.y = vertices[indices[3 * i + 2] + 1];
		point3.z = vertices[indices[3 * i + 2] + 2];

		vec3 vec1, vec2;
		vec1.subtract(point2, point1);
		vec2.subtract(point3, point2);

		vec3 normal;
		normal.cross(vec1, vec2);
		normal.normalize();

		surfNormals[i].x = normal.x;
		surfNormals[i].y = normal.y;
		surfNormals[i].z = normal.z;
	}

	// 頂点法線ベクトル配列を確保する。431,364バイト必要。
	// 使用後、delete[]すること。
	const int normNum = vertNum;
	float *vertNormals = new float[vertNum * 3];

	// 頂点法線ベクトルを求める
	vector<vector<unsigned int>> point(vertNum, vector<unsigned int>(10, 0));

	for (int i = 0; i < idxNum; i++) {
		unsigned int p1 = indices[3 * i + 0];
		unsigned int p2 = indices[3 * i + 1];
		unsigned int p3 = indices[3 * i + 2];

		point[p1].push_back(i);
		point[p2].push_back(i);
		point[p3].push_back(i);
	}

	for (int i = 0; i < normNum; i++) {
		vec3 vertNormal(0.0f, 0.0f, 0.0f);

		for (unsigned int j = 0; j < point[i].size(); j++) {
			vec3 tmp(surfNormals[point[i][j]].x, 
			         surfNormals[point[i][j]].y, 
			         surfNormals[point[i][j]].z);
			vertNormal.add(vertNormal, tmp);
		}

		vertNormal.normalize();

		vertNormals[3 * i + 0] = vertNormal.x;
		vertNormals[3 * i + 1] = vertNormal.y;
		vertNormals[3 * i + 2] = vertNormal.z;
	}

	// 面法線ベクトル配列をdelete[]する
	delete[] surfNormals;
	surfNormals = NULL;

	// 頂点配列を返す
	b->vertices = vertices;
	b->vertexNum = vertNum;

	// 頂点インデックス配列を返す
	b->vertexIndices = indices;
	b->indexNum = idxNum;

	// 法線ベクトル配列を返す
	b->normalVectors = vertNormals;
	b->vectorNum = normNum;

	return 0;
}

static int freeBunny(bunny *b) {
	delete[] b->vertices;
	b->vertices = NULL;
	b->vertexNum = 0;

	delete[] b->vertexIndices;
	b->vertexIndices = NULL;
	b->indexNum = 0;

	delete[] b->normalVectors;
	b->normalVectors = NULL;
	b->vectorNum = 0;

	return 0;
}

static void createOrthogonal(float Left, float Right, float Top, float Bottom, float Near, float Far, mat4 &m) {
	m.matrix[0] = 2.0f / (Right - Left);
	m.matrix[1] = 0.0f;
	m.matrix[2] = 0.0f;
	m.matrix[3] = -(Right + Left) / (Right - Left);
	m.matrix[4] = 0.0f;
	m.matrix[5] = 2.0f / (Top - Bottom);
	m.matrix[6] = 0.0f;
	m.matrix[7] = -(Top + Bottom) / (Top - Bottom);
	m.matrix[8] = 0.0f;
	m.matrix[9] = 0.0f;
	m.matrix[10] = -2.0f / (Far - Near);
	m.matrix[11] = -(Far + Near) / (Far - Near);
	m.matrix[12] = 0.0f;
	m.matrix[13] = 0.0f;
	m.matrix[14] = 0.0f;
	m.matrix[15] = 1.0f;

	return;
}

static void createLookAt(vec3 &position, vec3 &orientation, vec3 &up, mat4 &m) {
	vec3 v;

	v.cross(orientation, up);

	float m3 = -vec3::dot(position, orientation);
	float m7 = -vec3::dot(position, up);
	float m11 = -vec3::dot(position, v);

	m.matrix[0] = orientation.x;
	m.matrix[1] = orientation.y;
	m.matrix[2] = orientation.z;
	m.matrix[3] = m3;
	m.matrix[4] = up.x;
	m.matrix[5] = up.y;
	m.matrix[6] = up.z;
	m.matrix[7] = m7;
	m.matrix[8] = v.x;
	m.matrix[9] = v.y;
	m.matrix[10] = v.z;
	m.matrix[11] = m11;
	m.matrix[12] = 0.0f;
	m.matrix[13] = 0.0f;
	m.matrix[14] = 0.0f;
	m.matrix[15] = 1.0f;

	return;
}

static int split(string str, const char splitChar, vector<string> **splitedStr) {
	vector<string> *result = new vector<string>;
	int p = 0;

	for (int i = 0; i < str.length(); i++) {
		if (str[i] == splitChar) {
			result->push_back(str.substr(p, i - p));
			p = i + 1;
		}
	}

	result->push_back(str.substr(p, str.length() - p));

	*splitedStr = result;

	return 0;
}