// for Windows
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <stdio.h>
#include "Bunny.h"
#include "vec3.h"

using namespace std;

Bunny::Bunny(const char *filename) {
	int vertNum = 0;
	int idxNum = 0;

	//ファイルオープン
	ifstream file(filename);
	if (file.fail()) {
		throw BunnyException("error: ifstream constructor\n");
		return;
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
		int returnValue;
		if ((*v)[0] == "element") {
			if ((*v)[1] == "vertex") {
				returnValue = sscanf(&((*v)[2])[0], "%d", &vertNum);
				if (returnValue == EOF) {
					throw BunnyException("error: sscanf\n");
					return;
				}
			}
			if ((*v)[1] == "face") {
				returnValue = sscanf(&((*v)[2])[0], "%d", &idxNum);
				if (returnValue == EOF) {
					throw BunnyException("error: sscanf\n");
					return;
				}
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
		throw BunnyException("error: new[]");
		return;
	}

	// ひたすら読み込んでいく
	float x, y, z, confidence, intensity;
	for (int i = 0; i < vertNum; i++) {
		string str;
		getline(file, str);
		int returnValue = sscanf(&str[0], "%f %f %f %f %f", &x, &y, &z, &confidence, &intensity);
		if (returnValue == EOF) {
			throw BunnyException("error: sscanf\n");
			return;
		}
		vertices[3 * i + 0] = x;
		vertices[3 * i + 1] = y;
		vertices[3 * i + 2] = z;
	}

	// 頂点インデックス配列を確保する。69451 * 3 * 4 = 833,412バイト必要。
	// 使用後、delete[]すること。
	unsigned int *indices = new unsigned int[idxNum * 3];
	if (indices == NULL) {
		throw BunnyException("error: new[]");
		return;
	}

	// ひたすら読み込む
	int tmp, ix, iy, iz;
	for (int i = 0; i < idxNum; i++) {
		string str;
		getline(file, str);
		int returnValue = sscanf(&str[0], "%d %d %d %d", &tmp, &ix, &iy, &iz);
		if (returnValue == EOF) {
			throw BunnyException("error: sscanf\n");
			return;
		}
		indices[3 * i + 0] = ix;
		indices[3 * i + 1] = iy;
		indices[3 * i + 2] = iz;
	}

	// カラー配列を作成する
	// とりあえず全身白にしておく
	float *colors = new float[vertNum * 4];
	if (colors == NULL) {
		throw BunnyException("error: new[]");
		return;
	}

	for (int i = 0; i < vertNum; i++) {
		colors[4 * i + 0] = 1.0f; // R
		colors[4 * i + 1] = 1.0f; // G
		colors[4 * i + 2] = 1.0f; // B
		colors[4 * i + 3] = 1.0f; // A
	}

	// 面法線ベクトルを三角形から計算する
	vec3 *surfNormals = new vec3[idxNum];
	if (surfNormals == NULL) {
		throw BunnyException("error: new[]");
		return;
	}

	for (int i = 0; i < idxNum; i++) {
		vec3 point1, point2, point3;
		point1.x = vertices[3 * indices[3 * i + 0] + 0];
		point1.y = vertices[3 * indices[3 * i + 0] + 1];
		point1.z = vertices[3 * indices[3 * i + 0] + 2];

		point2.x = vertices[3 * indices[3 * i + 1] + 0];
		point2.y = vertices[3 * indices[3 * i + 1] + 1];
		point2.z = vertices[3 * indices[3 * i + 1] + 2];

		point3.x = vertices[3 * indices[3 * i + 2] + 0];
		point3.y = vertices[3 * indices[3 * i + 2] + 1];
		point3.z = vertices[3 * indices[3 * i + 2] + 2];

		vec3 vec1, vec2;
		vec1.subtract(point2, point1);
		vec2.subtract(point3, point2);

		vec3 normal;
		normal.cross(vec1, vec2);
		try {
			normal.normalize();
		}
		catch (ZeroDivideException &e) {
			printf("%s", &(e.getErrorMessage())[0]);
			return;
		}

		surfNormals[i].x = normal.x;
		surfNormals[i].y = normal.y;
		surfNormals[i].z = normal.z;

		// normal.print();
	}

	// 頂点法線ベクトル配列を確保する。431,364バイト必要。
	// 使用後、delete[]すること。
	const int normNum = vertNum;
	float *vertNormals = new float[normNum * 3];

	// 頂点法線ベクトルを求める
	vector<vector<unsigned int>> point(vertNum, vector<unsigned int>(0, 0));

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

		try {
			vertNormal.normalize();
		}
		catch (ZeroDivideException &e) {
			printf("%s", &(e.getErrorMessage())[0]);
			return;
		}

		vertNormals[3 * i + 0] = vertNormal.x;
		vertNormals[3 * i + 1] = vertNormal.y;
		vertNormals[3 * i + 2] = vertNormal.z;
	}

	// 面法線ベクトル配列をdelete[]する
	delete[] surfNormals;
	surfNormals = NULL;

	// 頂点配列を返す
	this->vertices = vertices;
	this->vertexNum = vertNum * 3;

	// カラー配列を返す
	this->colors = colors;
	this->colorNum = vertNum * 4;

	// 頂点インデックス配列を返す
	this->vertexIndices = indices;
	this->indexNum = idxNum * 3;

	// 法線ベクトル配列を返す
	this->normalVectors = vertNormals;
	this->vectorNum = normNum * 3;
}

Bunny::~Bunny() {
	delete[] this->vertices;
	this->vertices = NULL;
	this->vertexNum = 0;

	delete[] this->vertexIndices;
	this->vertexIndices = NULL;
	this->indexNum = 0;

	delete[] this->colors;
	this->colors = NULL;
	this->colorNum = 0;

	delete[] this->normalVectors;
	this->normalVectors = NULL;
	this->vectorNum = 0;

	return;
}

void Bunny::split(string str, const char splitChar, vector<string> **splitedStr) {
	vector<string> *result = new vector<string>;
	int p = 0;

	for (unsigned int i = 0; i < str.length(); i++) {
		if (str[i] == splitChar) {
			result->push_back(str.substr(p, i - p));
			p = i + 1;
		}
	}

	result->push_back(str.substr(p, str.length() - p));

	*splitedStr = result;

	return;
}
