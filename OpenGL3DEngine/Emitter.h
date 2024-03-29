#pragma once

class Emitter {
private:

	bool usesTexture;
	glm::vec2 TexCoords;
	Texture* texture = nullptr;
public:
	Light light;
	Material material;

	bool isEmitter = false;
	// default rgba value is ambient value
	Emitter(
		Light _light = Light(),
		Material _material = Material(),
		bool _usesTexture = false,
		Texture* _texture = nullptr
	) {
		light = _light;
		material = Material();
		usesTexture = _usesTexture;
		TexCoords = glm::vec2(0);
		texture = _texture;

		if (light.diffuse != glm::vec3(0.f)) {
			isEmitter = true;
		}
	}
};