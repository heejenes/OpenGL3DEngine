#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include "Vertex.h"

static void LoadVertexShader(const char* vertexShaderSource, unsigned int& vertexShader) {
	
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
}

static void LoadFragShader(const char* fragmentShaderSource, unsigned int& fragmentShader) {

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
}

static void LoadShaders(unsigned int& shaderProgram, unsigned int& vertexShader, unsigned int& fragmentShader) {
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
}

static void LoadVertices(const std::vector<Vertex>& vertexData) {

	// VBO (vertex buffer object) is of type GL_ARRAY_BUFFER
	uint32_t VBO;
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData.data(), GL_STATIC_DRAW);
}

static DrawDetails UploadMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t> element) {
	if (vertices.empty() || element.empty()) {
		throw ("empty vector");
	}
	// VBO: Vertex buffer objects: where the verticies are stored on the GPU
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