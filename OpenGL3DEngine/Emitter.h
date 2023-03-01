#pragma once

class Emitter {
public:
	glm::vec3 rgb;
	bool usesTexture;
	glm::vec2 TexCoords;
	Texture* texture;

	Emitter(glm::vec3 _rgb = glm::vec3(0), bool _usesTexture = false) {
		rgb = _rgb;
		usesTexture = _usesTexture;
	}
};