#pragma once

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 rgb;
	Vertex() {
		Position = glm::vec3(0.f);
		Normal = glm::vec3(0.f);
		TexCoords = glm::vec2(0.f);
		rgb = glm::vec3(1.f);
	}
	Vertex(float x, float y, float z, float nx, float ny, float nz, float s, float t, float r = 1, float g = 1, float b = 1) {
		Position[0] = x;
		Position[1] = y;
		Position[2] = z;

		Normal[0] = nx;
		Normal[1] = ny;
		Normal[2] = nz;

		TexCoords[0] = s;
		TexCoords[1] = t;

		rgb[0] = r;
		rgb[1] = g;
		rgb[2] = b;
	}
};
