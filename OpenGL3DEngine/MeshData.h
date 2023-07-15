#pragma once

struct MeshData {
	int sizeV, sizeT, sizeN;
	float* vertices;
	float* texCoords;
	float* normals;
};