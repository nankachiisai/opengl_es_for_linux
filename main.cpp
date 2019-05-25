// for Windows
#define _CRT_SECURE_NO_WARNINGS

#include <GL/glew.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include "Bunny.h"
#include "vec3.h"
#include "mat4.h"
#include "Exception.h"
#include "ShaderSource.h"
#include "CompiledShader.h"
#include "AttachedShader.h"

using namespace std;

static void display(void);
static void idle(void);
static GLuint transferData(void *data, int num, GLenum bufferType);
static void bindAttributeVariable(GLuint program, GLuint VBO, const char *name);
static void bindUniformVariable4x4(GLuint program, float *data, const char *name);
static void createOrthogonal(float Left, float Right, float Top, float Bottom, float Near, float Far, mat4 &m);
static void createLookAt(vec3 &position, vec3 &orientation, vec3 &up, mat4 &m);

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

static Bunny *b;

static AttachedShader *as;
static GLuint VBO; // Vertex Buffer Object
static GLuint IBO; // Index Buffer Object
static GLuint v_color;
static GLuint normalVector;

int main(int argc, char *argv[]) {
	GLenum err;

	// スタンフォードバニーの読み込み
	try {
		b = new Bunny("bun_zipper.ply");
//		b = new Bunny("test.ply");
	}
	catch (BunnyException &e) {
		printf("%s", &(e.getErrorMessage())[0]);
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
	try {
		ShaderSource vs("main.vert", GL_VERTEX_SHADER);
		ShaderSource fs("main.frag", GL_FRAGMENT_SHADER);

		VertexShader *cvs = (VertexShader *) vs.compile();
		FragmentShader *cfs = (FragmentShader *) fs.compile();

		as = new AttachedShader(*cvs, *cfs);
	}
	catch (Exception &e) {
		cout << e.getErrorMessage() << endl;
	}

	// 頂点配列をGPUに転送する
	float *vertices;
	int vertexNum;
	b->getVertex(&vertices, &vertexNum);
	VBO = transferData(vertices, vertexNum, GL_ARRAY_BUFFER);

	// カラー配列をGPUに転送する
	float *color;
	int colorNum;
	b->getColor(&color, &colorNum);
	v_color = transferData(color, colorNum, GL_ARRAY_BUFFER);

	// 法線ベクトル配列をGPUに転送する
	float *normalVectors;
	int vectorNum;
	b->getNormal(&normalVectors, &vectorNum);
	normalVector = transferData(normalVectors, vectorNum, GL_ARRAY_BUFFER);

	// インデックスをGPUに転送する
	unsigned int *vertexIndices;
	int indexNum;
	b->getIndex(&vertexIndices, &indexNum);
	IBO = transferData(vertexIndices, indexNum, GL_ELEMENT_ARRAY_BUFFER);

	// VBOとバーテックスシェーダのin変数とを関連付ける
	bindAttributeVariable(as->getProgramNumber(), VBO, "position");
	bindAttributeVariable(as->getProgramNumber(), v_color, "v_color");
	bindAttributeVariable(as->getProgramNumber(), normalVector, "normal");

	// メインループ
	glutMainLoop();

	// シェーダオブジェクト、シェーダプログラムを破棄する
	as->~AttachedShader();

	// バッファを削除する
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &v_color);
	glDeleteBuffers(1, &normalVector);
	glDeleteBuffers(1, &IBO);

	// スタンフォードバニーを削除する
	b->~Bunny();

	return 0;
}

static void display(void) {
	static float degree = 0.0f;

	// ウィンドウを白で塗りつぶす
	GLfloat white[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glClearBufferfv(GL_COLOR, 0, white);

	// 平行投影変換行列を生成する
	mat4 orthogonalMatrix;
	createOrthogonal(-0.5f, 0.5f, 0.5f, -0.5f, 0.0f, 10.0f, orthogonalMatrix);
	
	// 視野変換行列を生成する
	vec3 position(0.0f, 0.0f, 1.0f);
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
	tmpMatrix.multiply(orthogonalMatrix, lookAtMatrix);
	resultMatrix.multiply(tmpMatrix, transformMatrix);
	bindUniformVariable4x4(as->getProgramNumber(), resultMatrix.matrix, "transformMatrix");

	// 変換行列の逆行列を生成する
	mat4 invMatrix(resultMatrix);
	invMatrix.inverse();

	// 逆行列をuniform変数に関連付ける
	bindUniformVariable4x4(as->getProgramNumber(), invMatrix.matrix, "invMatrix");

	// 描画
	unsigned int *indices;
	int indexNum;
	b->getIndex(&indices, &indexNum);
	glDrawElements(GL_TRIANGLES, indexNum, GL_UNSIGNED_INT, 0);
	// glPointSize(10.0f);
	// glDrawArrays(GL_POINTS, 0, b.vertexNum);

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

static GLuint transferData(void *data, int num, GLenum bufferType) {
	// バッファを作成する
	GLuint buffer;
	glGenBuffers(1, &buffer);

	// バッファにデータをセットする
	glBindBuffer(bufferType, buffer);
	glBufferData(bufferType, sizeof(float) * num, data, GL_STATIC_DRAW);

	// バッファを返す
	return buffer;
}

static void bindAttributeVariable(GLuint program, GLuint VBO, const char *name) {
	// VBOをバインドする
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// VBOとin変数を関連付ける
	GLint attr = glGetAttribLocation(program, name);
	glEnableVertexAttribArray(attr);
	glVertexAttribPointer(attr, 3, GL_FLOAT, GL_FALSE, 0, 0);

	return;
}

static void bindUniformVariable4x4(GLuint program, float *data, const char *name) {
	// uniform変数の場所をgetする
	GLint uniform = glGetUniformLocation(program, name);

	// 4x4の行列をuniform変数に関連付ける
	glUniformMatrix4fv(uniform, 1, GL_FALSE, data);

	return;
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