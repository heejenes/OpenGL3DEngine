#pragma once

static void DrawVertices(unsigned int shaderProgram, unsigned int VAO, int indicesSize) {
	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}