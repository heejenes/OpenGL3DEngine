#pragma once

struct Vertex {
	float Pos[3] = { 0.f, 0.f, 0.f };

	Vertex(float x, float y, float z) {
		Pos[0] = x;
		Pos[1] = y;
		Pos[2] = z;
	}
};
