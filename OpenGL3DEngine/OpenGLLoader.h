#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include "Vertex.h"

static unsigned int LoadVertexShader(const char* vertexShaderSource) {
	unsigned int vertexShader;
	// Create shader of type GL_VERTEX_SHADER and store id in vertexShader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// link shader source code to shader in memory
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// compile shader
	glCompileShader(vertexShader);
	// look for errors
	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		writeLog("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");
		writeLog(infoLog);
	}
	return vertexShader;
}

static unsigned int LoadFragShader(const char* fragmentShaderSource) {
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// look for errors
	int  success;
	char infoLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		writeLog("ERROR::SHADER::FRAG::COMPILATION_FAILED\n");
		writeLog(infoLog);
	}
	return fragmentShader;
}

static unsigned int LoadShaders(unsigned int& vertexShader, unsigned int& fragmentShader) {
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// look for errors
	int  success;
	char infoLog[512];
	glGetShaderiv(shaderProgram, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
		writeLog("ERROR::SHADER::ATTACHING::COMPILATION_FAILED\n");
		writeLog(infoLog);
	}
	// Makes any shaders/rendering after this point be done with our shaders
	glUseProgram(shaderProgram);
	// clears objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return shaderProgram;
}

static void LoadVertices(const std::vector<Vertex>& vertexData, std::vector<unsigned int> indices, unsigned int& VBO, unsigned int& VAO, unsigned int& EBO) {

	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// creates a buffer objects in memory and assigns VBO to a buffer ID
	// that can be used to access that object. The first parameter is for
	// number of buffer items to create
	glGenBuffers(1, &VBO);
	// We can "bind" to several different buffers but only one of each type
	// bind generated buffer to GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// copy the vertexData to the buffer.
	// 4th parameter takes how GPU should manage data
	// OPTIONS:
	// GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
	// GL_STATIC_DRAW: the data is set only once and used many times.
	// GL_DYNAMIC_DRAW: the data is changed a lot and used many times.
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(Vertex), vertexData.data(), GL_STATIC_DRAW);

	// for EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	// Tell OpenGL how we've organized our vertices data (Looks at vertices data from the GL_ARRAY_BUFFER VBO)
	// Arg 1: which vertex attribute to configure ("location = 0" from vertexShaderSource)
	// Arg 2: size of vertex. 3 for x, y, z
	// Arg 3: Type of data (x, y, z are floats)
	// Arg 4: if we want data to be "normalized" (different from NDC, just ignore for now)
	// Arg 5: stride (number of bits between vertices = 3*(size of float))
	// Arg 6: "offset of where the position data begins in buffer" learn more later.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}
