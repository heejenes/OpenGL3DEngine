#pragma once

struct Material {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
	Material(
		glm::vec3 _ambient = glm::vec3(1.0f), 
		glm::vec3 _diffuse = glm::vec3(1.f), 
		glm::vec3 _specular = glm::vec3(1.f),
		float _shininess = 32
	) {
		ambient = _ambient;
		diffuse = _diffuse;
		specular = _specular;
		shininess = _shininess;
	}
};
