#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Callbacks.h"
#include "UtilityFunctions.h"
#include "Vertex.h"
#include "OpenGLLoader.h"
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

	std::vector<unsigned int> indices{
		0, 1, 3, 1, 2, 3
	};

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	// 1. bind Vertex Array Object
	glBindVertexArray(VAO);

	// VBO (vertex buffer object) is of type GL_ARRAY_BUFFER
	uint32_t VBO;
	LoadVertices(vertexData, VBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);


	unsigned int vertexShader;
	const char* vertexShaderSource =
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";
	LoadVertexShader(vertexShaderSource, vertexShader);

	unsigned int fragmentShader;
	const char* fragShaderSource =
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\0";
	LoadFragShader(fragShaderSource, fragmentShader);


	unsigned int shaderProgram;
	LoadShaders(shaderProgram, vertexShader, fragmentShader);


	// Tell OpenGL how we've organized our vertices data (Looks at vertices data from the GL_ARRAY_BUFFER VBO)
	// Arg 1: which vertex attribute to configure ("location = 0" from vertexShaderSource)
	// Arg 2: size of vertex. 3 for x, y, z
	// Arg 3: Type of data (x, y, z are floats)
	// Arg 4: if we want data to be "normalized" (different from NDC, just ignore for now)
	// Arg 5: stride (number of bits between vertices = 3*(size of float))
	// Arg 6: "offset of where the position data begins in buffer" learn more later.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	while (!glfwWindowShouldClose(window)) {
		// input handling
		processInput(window);

		// rendering
		// 
		// clears specified buffer with color specified in glClearColor(). 
		// (options: GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT, GL_STENCIL_BUFFER_BIT)
		glClear(GL_COLOR_BUFFER_BIT);
		// draws vertices
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO); 
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

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