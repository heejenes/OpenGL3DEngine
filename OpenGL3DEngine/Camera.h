#pragma once

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class Camera {
public:
	glm::vec3 cameraPos;
	glm::vec3 cameraDirection;
	glm::vec3 cameraUp;
	glm::vec3 cameraForward;
	glm::vec3 cameraRight;
	float aspectRatio, near, far;
	// euler Angles
	float Yaw;
	float Pitch;

	float MovementSpeed = 4.5f;
	float MouseSensitivity = 0.1f;
	// fovY in degrees
	float Zoom = 45.0f;

	glm::vec2 localCorner;
	glm::vec3 N, S, W, E;

	Camera(
		glm::vec3 _cameraPos = glm::vec3(0.0f, 0.0f, 3.0f),
		float _aspectRatio = 800.0f / 600.0f,
		float _near = 0.1f,
		float _far = 100.0f,
		float _Zoom = 45
	) {
		Yaw = 90.0f;
		Pitch = 0.0f;
		cameraPos = _cameraPos;
		localCorner = glm::vec2(
			2.f * glm::tan(glm::atan(_aspectRatio * glm::tan(glm::radians(Zoom)))),
			2.f * glm::tan(glm::radians(Zoom))
		);
		updateCameraVectors();
		updateConfig(_aspectRatio, _near, _far, _Zoom);
	}
	void updateCamera(std::vector<Shader> allShaders) {

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(Pitch), cameraRight);
		model = glm::rotate(model, glm::radians(Yaw - 90.f), glm::vec3(0, -1, 0));

		glm::vec3 NE, SE, SW, NW;
		NE = glm::vec3(model * glm::vec4(-localCorner.x, localCorner.y, 2.f, 1.f));
		NW = glm::vec3(model * glm::vec4(localCorner.x, localCorner.y, 2.f, 1.f));
		SE = glm::vec3(model * glm::vec4(-localCorner.x, -localCorner.y, 2.f, 1.f));
		SW = glm::vec3(model * glm::vec4(localCorner.x, -localCorner.y, 2.f, 1.f));

		N = glm::normalize(glm::cross(NE, NW));
		S = glm::normalize(glm::cross(SW, SE));
		W = glm::normalize(glm::cross(NW, SW));
		E = glm::normalize(glm::cross(SE, NE));

		for (Shader shader : allShaders) {
			shader.use();
			shader.setVec3("viewPos", cameraPos);
			genViewMatrix(shader);
			genProjMatrix(shader);
		}
	}
	bool IsInFrustum(glm::vec3 pos) {
		glm::vec3 localPos = glm::normalize(pos - cameraPos);
		float dist = glm::distance(pos, cameraPos);

		float closeRadius = 20.f;
		// allow chunks around us
		if (dist < closeRadius) {
			return true;
		}

		float underMargin = 0.7f; // skinniest at 1.0, widest at -1.0
		// allow chunks under us
		if (glm::dot(localPos, glm::vec3(0, -1, 0)) > underMargin) {
			return true;
		}

		float margin = 0.5f;
		// if inside
		if (glm::dot(localPos, N) < margin && glm::dot(localPos, S) < margin
			&& glm::dot(localPos, W) < margin && glm::dot(localPos, E) < margin) {
			if (dist > near && dist < far) {
				return true;
			}
		}
		return false;
	}
	bool IsClose(glm::vec3 pos) {
		float dist = glm::distance(pos, cameraPos);

		float closeRadius = far * 0.7f;
		// allow chunks around us
		if (dist < closeRadius) {
			return true;
		}
		return false;
	}
	void updateCameraVectors() {
		// calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		cameraDirection = glm::normalize(front);
		// also re-calculate the Right and Up vector
		cameraRight = glm::normalize(glm::cross(cameraDirection, glm::vec3(0.0f, 1.0f, 0.0f)));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		cameraUp = glm::normalize(glm::cross(cameraRight, cameraDirection));
		cameraForward = glm::normalize(glm::cross(glm::vec3(0, 1.0f, 0), cameraRight));
	}
	void updateConfig(
		float _aspectRatio,
		float _near,
		float _far,
		float _Zoom
	) {
		aspectRatio = _aspectRatio;
		near = _near;
		far = _far;
		Zoom = _Zoom;
	}
	void genViewMatrix(Shader ourShader) {
		glm::mat4 view = glm::mat4(1.0f);

		// note that we're translating the scene in the reverse direction of where we want to move
		view = glm::lookAt(cameraPos, cameraPos + cameraDirection, cameraUp);
		int viewLoc = glGetUniformLocation(ourShader.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	}

	void genProjMatrix(Shader ourShader) {
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(Zoom), aspectRatio, near, far);

		int projectionLoc = glGetUniformLocation(ourShader.ID, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	}

	// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(Camera_Movement direction, float deltaTime)
	{
		float worldHeight = 50.f;
		float velocity = MovementSpeed * deltaTime;
		if (direction == FORWARD)
			cameraPos += cameraForward * velocity;
		if (direction == BACKWARD)
			cameraPos -= cameraForward * velocity;
		if (direction == LEFT)
			cameraPos -= cameraRight * velocity;
		if (direction == RIGHT)
			cameraPos += cameraRight * velocity;
		if (direction == UP)
			if (cameraPos.y < worldHeight) {
				cameraPos += glm::vec3(0, 1.f, 0) * velocity;
			}
		if (direction == DOWN)
			cameraPos -= glm::vec3(0, 1.f, 0) * velocity;
	}

	// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
	{
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw += xoffset;
		Pitch += yoffset;

		// make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}

		// update Front, Right and Up Vectors using the updated Euler angles
		updateCameraVectors();
	}

	// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(float yoffset)
	{
		Zoom -= (float)yoffset;
		if (Zoom < 1.0f)
			Zoom = 1.0f;
		if (Zoom > 45.0f)
			Zoom = 45.0f;
	}
};
