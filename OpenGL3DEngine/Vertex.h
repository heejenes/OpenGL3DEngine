#pragma once
#include <stdint.h>

struct Vertex {
	float Pos[3] = { 0.f, 0.f, 0.f };

	Vertex(float x, float y, float z) {
		Pos[0] = x;
		Pos[1] = y;
		Pos[2] = z;
	}
};

struct DrawDetails {
	// unsigned 32 bit ints
	uint32_t vao = 0;
	uint32_t numElements = 0;
	DrawDetails(uint32_t v, uint32_t e) {
		// vertex array element
		vao = v;
		numElements = e;
	}
};