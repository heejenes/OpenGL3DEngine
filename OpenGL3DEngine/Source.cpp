#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Callbacks.h"
#include "UtilityFunctions.h"
#include "Vertex.h"
#include "OpenGLLoader.h"
#include "OpenGLDraw.h"
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

	// Coordinates must be between -1 and 1 to appear on the screen. 
	// This is called "normalized device coordinates (NDC)"
	std::vector<Vertex> vertexData;
	// each line of code is a 3d point, the three lines make a triangle
	vertexData.emplace_back(.5f, .5f, 0.f);
	vertexData.emplace_back(.5f, -.5f, 0.f);
	vertexData.emplace_back(-.5f, -.5f, 0.0f);
	vertexData.emplace_back(-.5f, .5f, 0.f);
	vertexData.emplace_back(-.6f, -.6f, 0.0f);
	vertexData.emplace_back(-.5f, -.4f, 0.0f);

	std::vector<unsigned int> indices{
		0, 1, 3, 4, 2, 5
	};

	// loads vertices
	unsigned int VBO;
	// points to VBO data already preconfigured with vertex attribute pointers. 
	// Means that the attribute pointers must be configured only once for each
	// configuration to a different VAO while in the loop, the corresponding
	// VAO can be binded to.
	unsigned int VAO;
	// allows for rendering vertices based on indices by storing the vertex data
	// in GL_ELEMENT_ARRAY_BUFFER instead which holds the vertex of the corresponding
	// index
	unsigned int EBO;
	LoadVertices(vertexData, indices, VBO, VAO, EBO);

	const char* vertexShaderSource =
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";
	const char* fragShaderSource =
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\0";
	unsigned int vertexShader = LoadVertexShader(vertexShaderSource);
	unsigned int fragmentShader = LoadFragShader(fragShaderSource);
	unsigned int shaderProgram = LoadShaders(vertexShader, fragmentShader);

	while (!glfwWindowShouldClose(window)) {
		// input handling
		processInput(window);

		// rendering
		// 
		// clears specified buffer with color specified in glClearColor(). 
		// (options: GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT, GL_STENCIL_BUFFER_BIT)
		glClear(GL_COLOR_BUFFER_BIT);
		// draws vertices
		DrawVertices(shaderProgram, VAO, indices.size());

		// Swaps the front and back buffers. front buffer is the buffer 
		// that is displayed, back buffer is the new frame being drawn 
		// to in the meanwhile. They then swap to show the new frame.
		glfwSwapBuffers(window);
		// listens for events (events trigger callbacks)
		glfwPollEvents();

	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);

	glfwTerminate();
	return 0;

}