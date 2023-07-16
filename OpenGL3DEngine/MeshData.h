#pragma once
#include <vector>

struct MeshData {
	int sizeV, sizeT, sizeN, sizeI;
	float* vertices;
	float* texCoords;
	float* normals;
	unsigned int* indices;
};