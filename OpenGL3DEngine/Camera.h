#pragma once

class Camera {
public:
	glm::vec3 cameraPos;
	glm::vec3 cameraTarget;
	glm::vec3 cameraDirection;
	glm::vec3 cameraUp;
	glm::vec3 cameraRight;
	float aspectRatio, near, far;
	// degrees
	float fovY;
	Camera(
		glm::vec3 _cameraPos = glm::vec3(0.0f, 0.0f, 3.0f),
		glm::vec3 _cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f),
		float _aspectRatio = 800.0f / 600.0f,
		float _near = 0.1f,
		float _far = 100.0f,
		float _fovY = 45
	) {
		updateMembers(_cameraPos, _cameraTarget, _aspectRatio, _near, _far, _fovY);
	}
	void updateCamera(std::vector<Shader> allShaders) {
		for (Shader shader : allShaders) {
			shader.use();
			genViewMatrix(shader);
			genProjMatrix(shader);
		}
	}
	void updateMembers(
		glm::vec3 _cameraPos,
		glm::vec3 _cameraTarget,
		float _aspectRatio,
		float _near,
		float _far,
		float _fovY
	) {
		cameraPos = _cameraPos;
		cameraTarget = _cameraTarget;
		cameraDirection = glm::normalize(cameraPos - cameraTarget);
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		cameraRight = glm::normalize(glm::cross(up, cameraDirection));
		cameraUp = glm::cross(cameraDirection, cameraRight);
		aspectRatio = _aspectRatio;
		near = _near;
		far = _far;
		fovY = _fovY;
	}
	void genViewMatrix(Shader ourShader) {
		glm::mat4 view = glm::mat4(1.0f);

		const float radius = 10.0f;
		float camX = sin(glfwGetTime()) * radius;
		float camZ = cos(glfwGetTime()) * radius;

		// note that we're translating the scene in the reverse direction of where we want to move
		view = glm::lookAt(cameraPos, cameraPos + cameraDirection, cameraUp);
		int viewLoc = glGetUniformLocation(ourShader.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	}

	void genProjMatrix(Shader ourShader) {
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(fovY), aspectRatio, near, far);

		int projectionLoc = glGetUniformLocation(ourShader.ID, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	}
};
