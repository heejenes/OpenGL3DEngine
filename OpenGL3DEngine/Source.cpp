#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Callbacks.h"
#include "UtilityFunctions.h"
#include "Vertex.h"
#include <vector>

int main(int argc, char** argv) {
	glfwSetErrorCallback(glfwErrorCallback);
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	GLFWwindow* window = glfwCreateWindow(800, 600, extractVersion(argv[0]), nullptr, nullptr);
	if (window == NULL) {
		writeLog("Failed to create GLFW window\n");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		writeLog("Failed to initialize GLAD\n");
		return -1;
	}

	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetWindowCloseCallback(window, glfwWindowCloseCallback);

	glClearColor(.2f, .2f, .6f, 0.f);
	std::vector<DrawDetails> drawDetails;
	{
		std::vector<Vertex> objPts;
		// each line is a "point", the three lines make a triangle
		objPts.emplace_back(.5f, -.5f, 0.f);
		objPts.emplace_back(-.5f, -.5f, 0.f);
		objPts.emplace_back(0.f, .5f, 0.f);

		// element array buffer
		std::vector<uint32_t> elem = { 0, 1, 2 };

		drawDetails.push_back(UploadMesh(objPts, elem));
	}
	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		glClear(GL_COLOR_BUFFER_BIT);
		Draw(drawDetails);

		// Swaps the front and back buffers. front buffer is the buffer 
		// that is displayed, back buffer is the new frame being drawn 
		// to in the meanwhile. They then swap to show the new frame.
		glfwSwapBuffers(window);
		// listens for events (events trigger callbacks)
		glfwPollEvents();

	}

	glfwTerminate();
	return 0;

}