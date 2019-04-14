#include "mat4.h"
#include <string.h>

mat4::mat4() {
}

mat4::mat4(float m[16]) {
	memcpy(this->matrix, m, sizeof(float) * 16);
}

mat4::mat4(mat4 &m) {
	memcpy(this->matrix, m.matrix, sizeof(float) * 16);
}

void mat4::add(mat4 &a, mat4 &b) {
	this->matrix[0] = a.matrix[0] + a.matrix[0];
	this->matrix[1] = a.matrix[1] + a.matrix[1];
	this->matrix[2] = a.matrix[2] + a.matrix[2];
	this->matrix[3] = a.matrix[3] + a.matrix[3];
	this->matrix[4] = a.matrix[4] + a.matrix[4];
	this->matrix[5] = a.matrix[5] + a.matrix[5];
	this->matrix[6] = a.matrix[6] + a.matrix[6];
	this->matrix[7] = a.matrix[7] + a.matrix[7];
	this->matrix[8] = a.matrix[8] + a.matrix[8];
	this->matrix[9] = a.matrix[9] + a.matrix[9];
	this->matrix[10] = a.matrix[10] + a.matrix[10];
	this->matrix[11] = a.matrix[11] + a.matrix[11];
	this->matrix[12] = a.matrix[12] + a.matrix[12];
	this->matrix[13] = a.matrix[13] + a.matrix[13];
	this->matrix[14] = a.matrix[14] + a.matrix[14];
	this->matrix[15] = a.matrix[15] + a.matrix[15];
}

void mat4::subtract(mat4 &a, mat4 &b) {
	this->matrix[0] = a.matrix[0] - a.matrix[0];
	this->matrix[1] = a.matrix[1] - a.matrix[1];
	this->matrix[2] = a.matrix[2] - a.matrix[2];
	this->matrix[3] = a.matrix[3] - a.matrix[3];
	this->matrix[4] = a.matrix[4] - a.matrix[4];
	this->matrix[5] = a.matrix[5] - a.matrix[5];
	this->matrix[6] = a.matrix[6] - a.matrix[6];
	this->matrix[7] = a.matrix[7] - a.matrix[7];
	this->matrix[8] = a.matrix[8] - a.matrix[8];
	this->matrix[9] = a.matrix[9] - a.matrix[9];
	this->matrix[10] = a.matrix[10] - a.matrix[10];
	this->matrix[11] = a.matrix[11] - a.matrix[11];
	this->matrix[12] = a.matrix[12] - a.matrix[12];
	this->matrix[13] = a.matrix[13] - a.matrix[13];
	this->matrix[14] = a.matrix[14] - a.matrix[14];
	this->matrix[15] = a.matrix[15] - a.matrix[15];
}

void mat4::multiply(mat4 &a, mat4 &b) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			float tmp = 0.0f;

			for (int k = 0; k < 4; k++) {
				tmp += a.matrix[4 * i + k] * b.matrix[4 * k + j];
			}

			this->matrix[4 * i + j] = tmp;
		}
	}
}

void mat4::multiply(float a) {
	this->matrix[0] *= a;
	this->matrix[1] *= a;
	this->matrix[2] *= a;
	this->matrix[3] *= a;
	this->matrix[4] *= a;
	this->matrix[5] *= a;
	this->matrix[6] *= a;
	this->matrix[7] *= a;
	this->matrix[8] *= a;
	this->matrix[9] *= a;
	this->matrix[10] *= a;
	this->matrix[11] *= a;
	this->matrix[12] *= a;
	this->matrix[13] *= a;
	this->matrix[14] *= a;
	this->matrix[15] *= a;
}

void mat4::identity() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i == j) {
				this->matrix[4 * i + j] = 1.0f;
			}
			else {
				this->matrix[4 * i + j] = 0.0f;
			}
		}
	}
}

void mat4::inverse() {
	mat4 invMatrix;

	// http://www.slis.tsukuba.ac.jp/~fujisawa.makoto.fu/cgi-bin/wiki/index.php?%A5%AC%A5%A6%A5%B9%8E%A5%A5%B8%A5%E7%A5%EB%A5%C0%A5%F3%CB%A1 を参考にした。

	// 単位行列を生成
	invMatrix.identity();

	// ガウス・ジョルダン法で逆行列計算
	for (int k = 0; k < 4; k++) {
		double kk = this->matrix[4 * k + k];

		// 対角要素を1にするために、k行目のすべての要素をkkで割る
		for (int j = 0; j < 4; j++) {
			this->matrix[4 * k + j] /= kk;
			invMatrix.matrix[4 * k + j] /= kk;
		}

		// k列目の非対角要素を0にする
		for (int i = 0; i < 4; i++) {
			if (i == k) {
				continue;
			}

			double ik = this->matrix[4 * i + k];
			for (int j = 0; j < 4; j++) {
				this->matrix[4 * i + j] -= this->matrix[4 * k + j] * ik;
				invMatrix.matrix[4 * i + j] -= invMatrix.matrix[4 * k + j] * ik;
			}
		}
	}

	// 結果をコピー
	memcpy(this->matrix, invMatrix.matrix, sizeof(float) * 16);
	
}

void mat4::transpose() {
	float tmp[16];

	memcpy(tmp, this->matrix, sizeof(float) * 16);
	this->matrix[0] = tmp[0];
	this->matrix[1] = tmp[4];
	this->matrix[2] = tmp[8];
	this->matrix[3] = tmp[12];
	this->matrix[4] = tmp[1];
	this->matrix[5] = tmp[5];
	this->matrix[6] = tmp[9];
	this->matrix[7] = tmp[13];
	this->matrix[8] = tmp[2];
	this->matrix[9] = tmp[6];
	this->matrix[10] = tmp[10];
	this->matrix[11] = tmp[14];
	this->matrix[12] = tmp[3];
	this->matrix[13] = tmp[7];
	this->matrix[14] = tmp[11];
	this->matrix[15] = tmp[15];
}