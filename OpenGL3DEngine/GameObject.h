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
		model = glm::rotate(model, glm::radians(localOffset.angle), localOffset.rotationAxis);
		model = glm::translate(model, localOffset.pos);
		model = glm::scale(model, localOffset.scale);
		// world pos
		model = glm::rotate(model, glm::radians(transform.angle), transform.rotationAxis);
		model = glm::translate(model, transform.pos);
		model = glm::scale(model, transform.scale);
		return model;
	}
public:
	Model* objModel;
	// world pos
	Transform transform;
	Transform localOffset;
	Shader* shader;
	int drawType;
	GameObject(
		Model* _objModel,
		Shader* _shader,
		Transform _transform = Transform(),
		Transform _localOffset = Transform(),
		int _drawType = GL_TRIANGLES
	) {
		objModel = _objModel;
		for (Mesh* mesh : objModel->meshes) {
			mesh->LoadVertexBuffers();
		}
		shader = _shader;
		transform = _transform;
		localOffset = _localOffset;
		drawType = _drawType;
	}
	Light GetEmitterLight() {
		for (Mesh* mesh : objModel->meshes) {
			return (mesh->emitter.light);
		}
	}
	glm::vec3 GetWorldPos() {
		// location of gameObject in world space.
		glm::vec4 worldPos = genModelMatrix() * glm::vec4(localOffset.pos, 1);
		return glm::vec3(worldPos);
	}
	void Draw(Light light, glm::vec3 emitterPos) {
		shader->use();
		for (Mesh* mesh : objModel->meshes) {
			for (int i = 0; i < mesh->textures.size(); i ++) {
				glActiveTexture(GL_TEXTURE0 + i);
				std::string name = "ourTexture";
				glUniform1i(glGetUniformLocation(shader->ID, name.c_str()), i);
				glBindTexture(GL_TEXTURE_2D, mesh->textures[i]->id);
			}

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
				glDrawElements(drawType, static_cast<unsigned int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
			}
			else {
				glDrawArrays(drawType, 0, mesh->indices[0]);
			}
		}
	}
};