#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "Vertex.h"

static DrawDetails UploadMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t> element) {
	if (vertices.empty() || element.empty()) {
		throw ("empty vector");
	}
	uint32_t VAO, VBO, EBO;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Pos));
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, element.size() * sizeof(uint32_t), element.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	return DrawDetails(VAO, element.size());
}

static void UnloadMesh(std::vector<DrawDetails>& details) {
	for (auto& d : details) {
		glDeleteBuffers(1, &d.vao);
	}
}