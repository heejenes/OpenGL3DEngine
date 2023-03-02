#pragma once

struct Light {
	// colors
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
	Light(
		glm::vec3 _ambient = glm::vec3(0.f),
		glm::vec3 _diffuse = glm::vec3(0.f),
		glm::vec3 _specular = glm::vec3(0.f)
	) {
		ambient = _ambient;
		diffuse = _diffuse;
		specular = _specular;
	}
};