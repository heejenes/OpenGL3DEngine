#pragma once

class GameObject {
private:
	glm::mat4 genAndAssignModelMatrix() {
		glm::mat4 model = genModelMatrix();
		int modelLoc = glGetUniformLocation(shader->ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		return model;
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
	glm::vec3 GetEmitterColor() {
		//std::cout << "emitter pos: " << worldPos[0] << " " << worldPos[1] << " " << worldPos[2] << std::endl;
		for (Mesh* mesh : objModel->meshes) {
			return (mesh->emitter.getEmitterColor());
		}
	}
	glm::vec3 GetWorldPos() {
		// location of gameObject in world space.
		glm::vec4 worldPos = genModelMatrix() * glm::vec4(localOffset.pos, 1);
		return glm::vec3(worldPos);
		//std::cout << "emitter pos: " << worldPos[0] << " " << worldPos[1] << " " << worldPos[2] << std::endl;
	}
	void Draw(glm::vec3 emitterPos, glm::vec3 emitterColor) {
		shader->use();
		for (Mesh* mesh : objModel->meshes) {
			for (int i = 0; i < mesh->textures.size(); i ++) {
				glActiveTexture(GL_TEXTURE0 + i);
				std::string name = "ourTexture";
				glUniform1i(glGetUniformLocation(shader->ID, name.c_str()), i);
				glBindTexture(GL_TEXTURE_2D, mesh->textures[i]->id);
			}

			genAndAssignModelMatrix();

			shader->setVec3("emitterColor", emitterColor);
			shader->setVec3("emitterPos", emitterPos);
			shader->setVec3("ambientColor", mesh->emitter.getAmbientColor());

			//Normal;
			glm::vec3 worldPos = GetWorldPos();

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