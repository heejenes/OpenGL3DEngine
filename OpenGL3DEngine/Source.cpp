#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>
#include <vector>
#include "Callbacks.h"
#include "Vertex.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "UtilityFunctions.h"
#include "Mesh.h"
#include "Transform.h"
#include "GameObject.h"

void processInput(GLFWwindow* window, Camera* camera) {
	// if user has pressed esc, close window
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	const float cameraSpeed = 0.05f; // adjust accordingly
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->cameraPos += cameraSpeed * camera->cameraDirection;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->cameraPos -= cameraSpeed * camera->cameraDirection;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->cameraPos -= glm::normalize(glm::cross(camera->cameraDirection, camera->cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->cameraPos += glm::normalize(glm::cross(camera->cameraDirection, camera->cameraUp)) * cameraSpeed;
}

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

	glClearColor(0.3f, 0.3f, 0.6f, 0.f);

	std::vector<GameObject> allGameObjects;
	std::vector<Shader> allShaders;
	Camera camera = Camera();

	// Coordinates must be between -1 and 1 to appear on the screen. 
	// This is called "normalized device coordinates (NDC)"
	std::vector<Vertex> vertexData;
	// each line of code is a 3d point, the three lines make a triangle
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	int stepSize = 5;
	int arrayLength = 36 * stepSize;
	for (int i = 0; i < arrayLength; i+=stepSize) {
		vertexData.emplace_back(
			vertices[i],
			vertices[i+1],
			vertices[i+2],
			1,
			1,
			1,
			vertices[i+3],
			vertices[i+4]
		);
	}

	std::vector<unsigned int> indices{36};

	Texture crateTexture("container.jpg");
	Mesh cubeMesh(vertexData, indices);
	Shader ourShader("vshader.glsl", "fshader.glsl");
	allShaders.push_back(ourShader);
	std::vector<Transform> cubeTransforms {
		Transform(glm::vec3(0.0f,  0.0f,  5.0f)),
		Transform(glm::vec3(2.0f,  5.0f, -15.0f)),
		Transform(glm::vec3(-1.5f, -2.2f, -2.5f)),
		Transform(glm::vec3(-3.8f, -2.0f, -12.3f)),
		Transform(glm::vec3(2.4f, -0.4f, -3.5f)),
		Transform(glm::vec3(-1.7f,  3.0f, -7.5f)),
		Transform(glm::vec3(1.3f, -2.0f, -2.5f)),
		Transform(glm::vec3(1.5f,  2.0f, -2.5f)),
		Transform(glm::vec3(1.5f,  0.2f, -1.5f)),
		Transform(glm::vec3(-1.3f,  1.0f, -1.5f))
	};
	for (int i = 0; i < cubeTransforms.size(); i++) {
		allGameObjects.push_back(
			GameObject(&cubeMesh, &ourShader, &crateTexture, cubeTransforms[i])
		);
	}
	// world axis
	Texture defaultTexture = Texture();
	std::vector<Vertex> xAxisPoints;
	xAxisPoints.emplace_back(-1, 0, 0, 1, 0, 0, 0, 1);
	xAxisPoints.emplace_back(1, 0, 0, 1, 0, 0, 0, 0);
	std::vector<unsigned int> xAxisIndices{ 2 };
	Mesh xAxisMesh(xAxisPoints, xAxisIndices);
	GameObject xAxis(&xAxisMesh, &ourShader, &defaultTexture, Transform(), Transform(), GL_LINES);
	allGameObjects.push_back(xAxis);

	std::vector<Vertex> yAxisPoints;
	yAxisPoints.emplace_back(0, -1, 0, 0, 1, 0, 0, 1);
	yAxisPoints.emplace_back(0, 1, 0, 0, 1, 0, 0, 0);
	std::vector<unsigned int> yAxisIndices{ 2 };
	Mesh yAxisMesh(yAxisPoints, yAxisIndices);
	GameObject yAxis(&yAxisMesh, &ourShader, &defaultTexture, Transform(), Transform(), GL_LINES);
	allGameObjects.push_back(yAxis);

	std::vector<Vertex> zAxisPoints;
	zAxisPoints.emplace_back(0, 0, -1, 0, 0, 1, 0, 1);
	zAxisPoints.emplace_back(0, 0, 1, 0, 0, 1, 0, 0);
	std::vector<unsigned int> zAxisIndices{ 2 };
	Mesh zAxisMesh(zAxisPoints, zAxisIndices);
	GameObject zAxis(&zAxisMesh, &ourShader, &defaultTexture, Transform(), Transform(), GL_LINES);
	allGameObjects.push_back(zAxis);

	while (!glfwWindowShouldClose(window)) {
		// input handling
		processInput(window, &camera);
		// Update shaders when SPACE is pressed
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			writeLog("RELOADING SHADER\n");
			ourShader.loadShader("vshader.glsl", "fshader.glsl");
		}

		// rendering
		// 
		// clears specified buffer with color specified in glClearColor(). 
		// (options: GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT, GL_STENCIL_BUFFER_BIT)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera.updateCamera(allShaders);
		// draws game objects
		for (int i = 0; i < allGameObjects.size(); i ++) {
			allGameObjects[i].Draw();
		}

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
