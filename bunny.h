#ifndef _BUNNY_H_
#define _BUNNY_H_

#include <string>
#include <vector>

class Bunny {
public:
	Bunny() {
		this->vertices = NULL;
		this->vertexNum = 0;

		this->vertexIndices = NULL;
		this->indexNum = 0;

		this->colors = NULL;
		this->colorNum = 0;

		this->normalVectors = NULL;
		this->vectorNum = 0;
	}

	Bunny(const char *filename);

	~Bunny();

	void getVertex(float **vertices, int *vertexNum) {
		*vertices = this->vertices;
		*vertexNum = this->vertexNum;
	}

	void getIndex(unsigned int **vertexIndices, int *indexNum) {
		*vertexIndices = this->vertexIndices;
		*indexNum = this->indexNum;
	}

	void getColor(float **color, int *colorNum) {
		*color = this->colors;
		*colorNum = this->colorNum;
	}

	void getNormal(float **normalVectors, int *vectorNum) {
		*normalVectors = this->normalVectors;
		*vectorNum = this->vectorNum;
	}

private:
	float *vertices;
	int vertexNum;

	unsigned int *vertexIndices;
	int indexNum;

	float *colors;
	int colorNum;

	float *normalVectors;
	int vectorNum;

	void split(std::string str, const char splitChar, std::vector<std::string> **splitedStr);
};

class BunnyException {
public:
	BunnyException(std::string errorMessage) {
		this->errorMessage = errorMessage;
	}

	std::string getErrorMessage() {
		return errorMessage;
	}

private:
	std::string errorMessage;
};

#endif