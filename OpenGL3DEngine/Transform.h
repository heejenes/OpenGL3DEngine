#pragma once

class Transform {
public:
	glm::vec3 pos, scale, rotationAxis;
	// degrees
	float angle;
	Transform(
		glm::vec3 _pos = glm::vec3(0, 0, 0), 
		glm::vec3 _scale = glm::vec3(1, 1, 1), 
		glm::vec3 _rotationAxis = glm::vec3(0, 1, 0),
		float _angle = 0
	) {
		pos = _pos;
		scale = _scale;
		rotationAxis = _rotationAxis;
		angle = _angle;
	}
};