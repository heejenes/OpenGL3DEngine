#pragma once

#ifndef OPENGLDRAW_H
#define OPENGLDRAW_H

#include "Shader.h"

static void DrawVertices(Shader shader, unsigned int texture, unsigned int VAO, int indicesSize, bool useIndices) {
	shader.use();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(VAO);
	if (useIndices) {
		glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, 0);
	}
	else {
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	glBindVertexArray(0);
	glBindVertexArray(1);
	glBindTexture(GL_TEXTURE_2D, texture);
}
#endif