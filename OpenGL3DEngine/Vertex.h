#pragma once

struct Vertex {
	float Pos[6] = { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f };

	Vertex(float x, float y, float z, float r, float g, float b) {
		Pos[0] = x;
		Pos[1] = y;
		Pos[2] = z;

		Pos[3] = r;
		Pos[4] = g;
		Pos[5] = b;
	}
};
