#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Transform.h"

class GrassGenerator {
private:
	int xSize;
	int ySize;
public:
	GrassGenerator(int _xSize, int _ySize) {
		xSize = _xSize;
		ySize = _ySize;
	}
	void GenerateModelMatrices() {
		glm::mat4* modelMatrices;
		srand(glfwGetTime());
		modelMatrices = new glm::mat4[(xSize * ySize)];
		for (int i = 0; i < (xSize * ySize); i++) {
			Transform temp(
				glm::vec3(
					0,
					// GET HEIGHT USING THE SAME NOISE FUNCTION AS THE TERRAIN
					0
				),
				glm::vec3(1),
				glm::vec3(0, 1, 0),
				0.f
			);

			modelMatrices[i] = temp.genModelMatrix();
		}
	}
};