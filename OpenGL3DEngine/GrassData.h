#pragma once
#include "MeshData.h"

struct GrassData : MeshData {
	float sizeX = 0.2f;
	float sizeZ = 0.05f;
	float height = 0.6f;
	GrassData() {
		sizeV = 17*3;
		sizeI = 30*3;
		vertices = new float[sizeV] {
			sizeX, height * 0.f, sizeZ,
			-sizeX, height * 0.f, sizeZ,
			-sizeX, height * 0.f, -sizeZ,
			sizeX, height * 0.f, -sizeZ,

			sizeX*0.9f, height * 1.f, sizeZ * 0.9f,
			-sizeX * 0.9f, height * 1.f, sizeZ * 0.9f,
			-sizeX * 0.9f, height * 1.f, -sizeZ * 0.9f,
			sizeX * 0.9f, height * 1.f, -sizeZ * 0.9f,

			sizeX * 0.6f, height * 2.8f, sizeZ * 0.6f,
			-sizeX * 0.6f, height * 2.8f, sizeZ * 0.6f,
			-sizeX * 0.6f, height * 2.8f, -sizeZ * 0.6f,
			sizeX * 0.6f, height * 2.8f, -sizeZ * 0.6f,

			sizeX * 0.3f, height * 3.5f, sizeZ * 0.3f,
			-sizeX * 0.3f, height * 3.5f, sizeZ * 0.3f,
			-sizeX * 0.3f, height * 3.5f, -sizeZ * 0.3f,
			sizeX * 0.3f, height * 3.5f, -sizeZ * 0.3f,

			0, height * 4.f, 0
		};
		indices = new unsigned int[sizeI] {
			0, 1, 2,
			0, 2, 3,

			1, 0, 4,
			4, 5, 1,
			2, 1, 5,
			5, 6, 2,
			3, 2, 6,
			6, 7, 3,
			0, 3, 7,
			7, 4, 0,

			1+4, 0+4, 4+4,
			4+4, 5+4, 1+4,
			2+4, 1+4, 5+4,
			5+4, 6+4, 2+4,
			3+4, 2+4, 6+4,
			6+4, 7+4, 3+4,
			0+4, 3+4, 7+4,
			7+4, 4+4, 0+4,

			1+8, 0+8, 4+8,
			4+8, 5+8, 1+8,
			2+8, 1+8, 5+8,
			5+8, 6+8, 2+8,
			3+8, 2+8, 6+8,
			6+8, 7+8, 3+8,
			0+8, 3+8, 7+8,
			7+8, 4+8, 0+8,

			4 + 8, 16, 5 + 8, 
			5 + 8, 16, 6 + 8, 
			6 + 8, 16, 7 + 8, 
			7 + 8, 16, 4 + 8
		};
	}
};