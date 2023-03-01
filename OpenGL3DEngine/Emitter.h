#pragma once

class Emitter {
private:
	// last value is intensity
	glm::vec4 ambientColor;
	glm::vec4 emitterColor;

	bool usesTexture;
	glm::vec2 TexCoords;
	Texture* texture;
public:
	bool isEmitter = false;
	// default rgba value is ambient value
	Emitter(
		glm::vec4 _emitterColor = glm::vec4(0), 
		bool _usesTexture = false
	) {
		emitterColor = _emitterColor;
		ambientColor = glm::vec4(1, 1, 1, 0.1);
		usesTexture = _usesTexture;
		TexCoords = glm::vec2(0);
		texture = NULL;

		if (getEmitterColor() != glm::vec3(0)) {
			isEmitter = true;
		}
	}

	glm::vec3 getAmbientColor() {
		return glm::vec3(ambientColor) * ambientColor[3];
	}

	glm::vec3 getEmitterColor() {
		return glm::vec3(emitterColor) * emitterColor[3];
	}
};