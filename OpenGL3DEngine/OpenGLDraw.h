#pragma once

#ifndef OPENGLDRAW_H
#define OPENGLDRAW_H

#include "Shader.h"

static void DrawVertices(Shader shader, unsigned int VAO, int indicesSize) {
	shader.use();
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glBindVertexArray(1);
}
#endif