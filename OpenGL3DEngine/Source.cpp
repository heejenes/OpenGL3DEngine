#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>
#include <vector>

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include "Vertex.h"
#include "Material.h"
#include "Light.h"

#include "UtilityFunctions.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Callbacks.h"
#include "Emitter.h"
#include "Mesh.h"
#include "Model.h"
#include "Transform.h"
#include "GameObject.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// camera

Camera camera = Camera();
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

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
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera->ProcessKeyboard(UP, deltaTime);
}

int main(int argc, char** argv) {
	srand(time(NULL));
	glfwSetErrorCallback(glfwErrorCallback);
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, extractVersion(argv[0]), nullptr, nullptr);
	if (window == NULL) {
		writeLog("Failed to create GLFW window\n");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetWindowCloseCallback(window, glfwWindowCloseCallback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		writeLog("Failed to initialize GLAD\n");
		return -1;
	}
	// tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
	stbi_set_flip_vertically_on_load(true);

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	glClearColor(0.3f, 0.3f, 0.6f, 0.f);
	
	std::vector<GameObject> allGameObjects;
	std::vector<GameObject> allEmitters;
	std::vector<Shader> allShaders;

	// Coordinates must be between -1 and 1 to appear on the screen. 
	// This is called "normalized device coordinates (NDC)"
	std::vector<Vertex> vertexData;
	// each line of code is a 3d point, the three lines make a triangle
	float verticesAndTexCoords[] = {
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
	float verticesAndNormals[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};
	
	int tex, norm;
	for (int i = 0; i < 36; i++) {
		tex = i * 5;
		norm = i * 6 + 3;
		vertexData.emplace_back(
			Vertex(
				verticesAndTexCoords[tex],
				verticesAndTexCoords[tex +1],
				verticesAndTexCoords[tex +2],
				verticesAndNormals[norm],
				verticesAndNormals[norm + 1],
				verticesAndNormals[norm + 2],
				verticesAndTexCoords[tex +3],
				verticesAndTexCoords[tex +4]
			)
		);
	}
	
	std::vector<unsigned int> indices{36};

	Shader ourShader = Shader("vshader.glsl", "fshader.glsl");
	allShaders.push_back(ourShader);
	Shader flatShader = Shader("vshader.glsl", "flatColorFrag.glsl");
	allShaders.push_back(flatShader);

	// world axis
	Texture defaultTexture = Texture();
	std::vector<Vertex> xAxisPoints;
	xAxisPoints.emplace_back(-1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0);
	xAxisPoints.emplace_back(1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0);
	std::vector<unsigned int> xAxisIndices{ 2 };
	Mesh xAxisMesh = Mesh(xAxisPoints, xAxisIndices, &defaultTexture);
	Model xAxisModel(&xAxisMesh);
	GameObject xAxis(&xAxisModel, &flatShader, Transform(), Transform(), GL_LINES);
	allGameObjects.push_back(xAxis);

	std::vector<Vertex> yAxisPoints;
	yAxisPoints.emplace_back(0, -1, 0, 0, 1, 0, 0, 1, 0, 1, 0);
	yAxisPoints.emplace_back(0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0);
	std::vector<unsigned int> yAxisIndices{ 2 };
	Mesh yAxisMesh = Mesh(yAxisPoints, yAxisIndices, &defaultTexture);
	Model yAxisModel(&yAxisMesh);
	GameObject yAxis(&yAxisModel, &flatShader, Transform(), Transform(), GL_LINES);
	allGameObjects.push_back(yAxis);

	std::vector<Vertex> zAxisPoints;
	zAxisPoints.emplace_back(0, 0, -1, 0, 0, 1, 0, 1, 0, 0, 1);
	zAxisPoints.emplace_back(0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1);
	std::vector<unsigned int> zAxisIndices{ 2 };
	Mesh zAxisMesh = Mesh(zAxisPoints, zAxisIndices, &defaultTexture);
	Model zAxisModel(&zAxisMesh);
	GameObject zAxis(&zAxisModel, &flatShader, Transform(), Transform(), GL_LINES);
	allGameObjects.push_back(zAxis);

	Texture crateTexture("container.jpg");
	Mesh cubeMesh = Mesh(vertexData, indices, &crateTexture);
	Model cubeModel(&cubeMesh);
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
		Transform(glm::vec3(-1.3f,  1.0f, -1.5f)),
		Transform(glm::vec3(-1.3f,  10.0f, -1.5f))
	};
	for (int i = 0; i < cubeTransforms.size(); i++) {
		allGameObjects.push_back(
			GameObject(&cubeModel, &ourShader, cubeTransforms[i])
		);
	}
	
	Emitter lightAEmitter(Light(
		glm::vec3(0.1f),
		glm::vec3(1.f),
		glm::vec3(1.0f),
		glm::vec4(1.0f, 1.f, 1.f, 0.f)
	));
	Mesh lightAMesh = Mesh(vertexData, indices, &crateTexture, lightAEmitter);
	Model lightAModel(&lightAMesh);
	GameObject lightA(&lightAModel, &flatShader, Transform(glm::vec3(1.2f, 1.0f, 2.0f), glm::vec3(0.3f)));
	allGameObjects.push_back(lightA);
	allEmitters.push_back(lightA);

	while (!glfwWindowShouldClose(window)) {

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input handling
		processInput(window, &camera);

		// rendering
		// 
		// clears specified buffer with color specified in glClearColor(). 
		// (options: GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT, GL_STENCIL_BUFFER_BIT)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera.updateCamera(allShaders);
		// draws game objects
		for (int i = 0; i < allGameObjects.size(); i ++) {
			// assuming shared shader and only one emmiter
			allGameObjects[i].Draw(
				allEmitters[0].GetEmitterLight(),
				allEmitters[0].GetWorldPos()
			);
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

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}