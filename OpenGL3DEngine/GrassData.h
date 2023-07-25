#pragma once
#include "MeshData.h"

struct GrassData : MeshData {
	float sizeX = 0.08f;
	float sizeZ = 0.02f;
	float height = 0.6f;
	GrassData() {
		sizeV = 64 * 3; // 192
		sizeI = 30 * 3;
		vertices = new float[sizeV] {
			//////////// FLOOR
			sizeX, height * 0.f, sizeZ,
			-sizeX, height * 0.f, sizeZ,
			-sizeX, height * 0.f, -sizeZ,
			sizeX, height * 0.f, -sizeZ,

			//////////////// FIRST ROW

			sizeX, height * 0.f, sizeZ,
			-sizeX, height * 0.f, sizeZ,
			sizeX * 0.9f, height * 1.f, sizeZ * 0.9f,
			-sizeX * 0.9f, height * 1.f, sizeZ * 0.9f,

			-sizeX, height * 0.f, sizeZ,
			-sizeX, height * 0.f, -sizeZ,
			-sizeX * 0.9f, height * 1.f, sizeZ * 0.9f,
			-sizeX * 0.9f, height * 1.f, -sizeZ * 0.9f,

			-sizeX, height * 0.f, -sizeZ,
			sizeX, height * 0.f, -sizeZ,
			-sizeX * 0.9f, height * 1.f, -sizeZ * 0.9f,
			sizeX * 0.9f, height * 1.f, -sizeZ * 0.9f,

			sizeX, height * 0.f, -sizeZ,
			sizeX, height * 0.f, sizeZ,
			sizeX * 0.9f, height * 1.f, -sizeZ * 0.9f,
			sizeX * 0.9f, height * 1.f, sizeZ * 0.9f,

			/////////////// SECOND ROW

			sizeX * 0.9f, height * 1.f, sizeZ * 0.9f,
			-sizeX * 0.9f, height * 1.f, sizeZ * 0.9f,
			sizeX * 0.6f, height * 2.8f, sizeZ * 0.6f,
			-sizeX * 0.6f, height * 2.8f, sizeZ * 0.6f,

			-sizeX * 0.9f, height * 1.f, sizeZ * 0.9f,
			-sizeX * 0.9f, height * 1.f, -sizeZ * 0.9f,
			-sizeX * 0.6f, height * 2.8f, sizeZ * 0.6f,
			-sizeX * 0.6f, height * 2.8f, -sizeZ * 0.6f,

			-sizeX * 0.9f, height * 1.f, -sizeZ * 0.9f,
			sizeX * 0.9f, height * 1.f, -sizeZ * 0.9f,
			-sizeX * 0.6f, height * 2.8f, -sizeZ * 0.6f,
			sizeX * 0.6f, height * 2.8f, -sizeZ * 0.6f,

			sizeX * 0.9f, height * 1.f, -sizeZ * 0.9f,
			sizeX * 0.9f, height * 1.f, sizeZ * 0.9f,
			sizeX * 0.6f, height * 2.8f, -sizeZ * 0.6f,
			sizeX * 0.6f, height * 2.8f, sizeZ * 0.6f,

			////////////////////// THIRD ROW

			sizeX * 0.6f, height * 2.8f, sizeZ * 0.6f,
			-sizeX * 0.6f, height * 2.8f, sizeZ * 0.6f,
			sizeX * 0.3f, height * 3.5f, sizeZ * 0.3f,
			-sizeX * 0.3f, height * 3.5f, sizeZ * 0.3f,

			-sizeX * 0.6f, height * 2.8f, sizeZ * 0.6f,
			-sizeX * 0.6f, height * 2.8f, -sizeZ * 0.6f,
			-sizeX * 0.3f, height * 3.5f, sizeZ * 0.3f,
			-sizeX * 0.3f, height * 3.5f, -sizeZ * 0.3f,

			-sizeX * 0.6f, height * 2.8f, -sizeZ * 0.6f,
			sizeX * 0.6f, height * 2.8f, -sizeZ * 0.6f,
			-sizeX * 0.3f, height * 3.5f, -sizeZ * 0.3f,
			sizeX * 0.3f, height * 3.5f, -sizeZ * 0.3f,

			sizeX * 0.6f, height * 2.8f, -sizeZ * 0.6f,
			sizeX * 0.6f, height * 2.8f, sizeZ * 0.6f,
			sizeX * 0.3f, height * 3.5f, -sizeZ * 0.3f,
			sizeX * 0.3f, height * 3.5f, sizeZ * 0.3f,

			//////////////////// FOURTH ROW

			sizeX * 0.3f, height * 3.5f, sizeZ * 0.3f,
			-sizeX * 0.3f, height * 3.5f, sizeZ * 0.3f,
			0, height * 4.f, 0,

			- sizeX * 0.3f, height * 3.5f, sizeZ * 0.3f,
			-sizeX * 0.3f, height * 3.5f, -sizeZ * 0.3f,
			0, height * 4.f, 0,

			- sizeX * 0.3f, height * 3.5f, -sizeZ * 0.3f,
			sizeX * 0.3f, height * 3.5f, -sizeZ * 0.3f,
			0, height * 4.f, 0,

			sizeX * 0.3f, height * 3.5f, -sizeZ * 0.3f,
			sizeX * 0.3f, height * 3.5f, sizeZ * 0.3f,
			0, height * 4.f, 0
		};
		indices = new unsigned int[sizeI];
		indices[0] = 0; // 0
		indices[1] = 1;
		indices[2] = 2;
		indices[3] = 0;
		indices[4] = 2;
		indices[5] = 3; // 3
		for (int i = 0; i < 12; i++) {
			int a = i * 6 + 6;
			int b = i * 4 + 4;
			indices[a + 0] = 2 + b;
			indices[a + 1] = 1 + b;
			indices[a + 2] = 0 + b; // 4, 8
			indices[a + 3] = 1 + b;
			indices[a + 4] = 2 + b;
			indices[a + 5] = 3 + b; // 7, 51
		}
		for (int i = 0; i < 4; i++) {
			int a = i * 3 + 78;
			int b = i * 3 + 52;
			indices[a + 0] = 2 + b; // 63
			indices[a + 1] = 1 + b;
			indices[a + 2] = 0 + b; // 52
		}
	}
};