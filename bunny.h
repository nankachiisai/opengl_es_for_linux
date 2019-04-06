#ifndef _BUNNY_H_
#define _BUNNY_H_

typedef struct bunny {
	float *vertices;
	int vertexNum;

	unsigned int *vertexIndices;
	int indexNum;

	float *normalVectors;
	int vectorNum;
} bunny;

#endif