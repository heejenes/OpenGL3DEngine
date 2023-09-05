#pragma once

class GameObject {
private:
	void genAndAssignModelMatrix() {
		glm::mat4 model = genModelMatrix();
		shader->use();
		shader->setMat4("model", model);
	}
	glm::mat4 genModelMatrix() {
		glm::mat4 model = glm::mat4(1.0f);
		// scale, translate, rotate
		// local
		model = glm::translate(model, localOffset.pos);
		model = glm::scale(model, localOffset.scale);
		model = glm::rotate(model, glm::radians(localOffset.angle), localOffset.rotationAxis);
		// world pos
		model = glm::translate(model, transform.pos);
		model = glm::scale(model, transform.scale);
		model = glm::rotate(model, glm::radians(transform.angle), transform.rotationAxis);
		return model;
	}
public:
	Model objModel;
	CubeMap* cubeMap = nullptr;
	bool usesCubeMap = false;
	// world pos
	Transform transform;
	Transform localOffset;
	glm::vec3 center;
	Shader* shader = nullptr;
	int drawType;
	bool isInstanced;
	int instanceWidth, instanceHeight;
	GameObject(
		Model _objModel = Model(0),
		Shader* _shader = nullptr,
		Transform _transform = Transform(),
		Transform _localOffset = Transform(),
		glm::vec3 _center = glm::vec3(0),
		int _drawType = GL_TRIANGLES,
		bool _isInstanced = false,
		int _instanceWidth = 0,
		int _instanceHeight = 0
	) {
		objModel = _objModel;
		shader = _shader;
		transform = _transform;
		localOffset = _localOffset;
		if (_center == glm::vec3(0)) {
			center = _transform.pos;
		}
		else {
			center = _center;
		}
		drawType = _drawType;
		isInstanced = _isInstanced;
		instanceWidth = _instanceWidth;
		instanceHeight = _instanceHeight;
	}
	GameObject(
		CubeMap* _cubeMap,
		Model _objModel = Model(0),
		Shader* _shader = nullptr,
		Transform _transform = Transform(),
		Transform _localOffset = Transform(),
		glm::vec3 _center = glm::vec3(0),
		int _drawType = GL_TRIANGLES
	) {
		cubeMap = _cubeMap;
		usesCubeMap = true;
		objModel = _objModel;
		shader = _shader;
		transform = _transform;
		localOffset = _localOffset;
		if (_center == glm::vec3(0)) {
			center = _transform.pos;
		}
		else {
			center = _center;
		}
		drawType = _drawType;
		isInstanced = false;
		instanceWidth = 0;
		instanceHeight = 0;
	}

	GameObject(const GameObject &_go) {
		cubeMap = _go.cubeMap;
		usesCubeMap = _go.usesCubeMap;
		objModel = _go.objModel;
		shader = _go.shader;
		transform = _go.transform;
		localOffset = _go.localOffset;
		if (_go.center == glm::vec3(0)) {
			center = transform.pos;
		}
		else {
			center = _go.center;
		}
		drawType = _go.drawType;
		isInstanced = _go.isInstanced;
		instanceWidth = _go.instanceWidth;
		instanceHeight = _go.instanceHeight;
	}

	void operator= (GameObject other) {
		cubeMap = other.cubeMap;
		usesCubeMap = other.usesCubeMap;
		objModel = other.objModel;
		shader = other.shader;
		transform = other.transform;
		localOffset = other.localOffset;
		if (other.center == glm::vec3(0)) {
			center = transform.pos;
		}
		else {
			center = other.center;
		}
		drawType = other.drawType;
		isInstanced = other.isInstanced;
		instanceWidth = other.instanceWidth;
		instanceHeight = other.instanceHeight;
	}

	Light GetEmitterLight() {
		for (Mesh* mesh : objModel.meshes) {
			return (mesh->emitter.light);
		}
	}
	glm::vec3 GetWorldPos() {
		// location of gameObject in world space.
		glm::vec4 worldPos = genModelMatrix() * glm::vec4(localOffset.pos, 1);
		return glm::vec3(worldPos);
	}
	void Draw(Light light, glm::vec3 emitterPos) {

		//std::cout << "START: " << glGetError() << std::endl;
		if (usesCubeMap) {
			glDepthFunc(GL_LEQUAL);
			shader->use();
			shader->setInt("skybox", 0);
			glBindVertexArray(objModel.meshes[0]->getVAO());
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap->id);
			glDrawArrays(drawType, 0, objModel.meshes[0]->indices[0]);
			glBindVertexArray(0);
			glDepthFunc(GL_LESS);
		}
		else {
			shader->use();
			//std::cout << "A: " << glGetError() << std::endl;
			for (Mesh* mesh : objModel.meshes) {

				for (int i = 0; i < mesh->textures.size(); i++) {
					glActiveTexture(GL_TEXTURE0 + i);
					std::string name = "ourTexture";
					glUniform1i(glGetUniformLocation(shader->ID, name.c_str()), i);
					glBindTexture(GL_TEXTURE_2D, mesh->textures[i]->id);
					//std::cout << "B: " << glGetError() << std::endl;
				}

				shader->setFloat("time", glfwGetTime());

				genAndAssignModelMatrix();

				shader->setVec3("light.ambient", light.ambient);
				shader->setVec3("light.diffuse", light.diffuse);
				shader->setVec3("light.specular", light.specular);
				if (light.lightDir.w == 0) {
					shader->setVec3("light.position", emitterPos);
					shader->setFloat("light.constant", light.coefficients.x);
					shader->setFloat("light.linear", light.coefficients.y);
					shader->setFloat("light.quadratic", light.coefficients.z);
				}
				else {
					shader->setVec4("light.lightDir", light.lightDir);
				}

				shader->setVec3("material.ambient", mesh->emitter.material.ambient);
				shader->setVec3("material.diffuse", mesh->emitter.material.diffuse);
				shader->setVec3("material.specular", mesh->emitter.material.specular);
				shader->setFloat("material.shininess", mesh->emitter.material.shininess);

				glBindVertexArray(mesh->getVAO());

				if (mesh->usesIndex) {
					if (isInstanced) {
						glDrawElementsInstanced(drawType, static_cast<unsigned int>(mesh->indices.size()), GL_UNSIGNED_INT, 0, instanceHeight * instanceWidth);
					}
					else {
						glDrawElements(drawType, static_cast<unsigned int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
					}
				}
				else {
					if (isInstanced) {
						glDrawArraysInstanced(drawType, 0, mesh->indices[0], instanceHeight * instanceWidth);
					}
					else {
						glDrawArrays(drawType, 0, mesh->indices[0]);
					}
				}
			}
		}
	}
};