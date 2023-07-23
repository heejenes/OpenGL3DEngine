#pragma once

class Transform {
public:
	glm::vec3 pos, scale, rotationAxis;
	// degrees
	float angle;
	Transform(
		glm::vec3 _pos = glm::vec3(0), 
		glm::vec3 _scale = glm::vec3(1), 
		glm::vec3 _rotationAxis = glm::vec3(0, 1, 0),
		float _angle = 0
	) {
		pos = _pos;
		scale = _scale;
		rotationAxis = _rotationAxis;
		angle = _angle;
	}
	glm::mat4 genModelMatrix() {
		glm::mat4 model = glm::mat4(1.0f);
		// scale, translate, rotate
		// local
		model = glm::translate(model, pos);
		model = glm::scale(model, scale);
		model = glm::rotate(model, glm::radians(angle), rotationAxis);
		return model;
	}

	glm::mat4 genScaleRotateModelMatrix() {
		glm::mat4 model = glm::mat4(1.0f);
		// scale, translate, rotate
		// local
		model = glm::scale(model, scale);
		model = glm::rotate(model, glm::radians(angle), rotationAxis);
		return model;
	}

};