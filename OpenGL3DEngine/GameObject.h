#pragma once

class GameObject {
private:
	void genModelMatrix() {
		glm::mat4 model = glm::mat4(1.0f);
		// scale, translate, rotate
		// local
		model = glm::scale(model, localOffset.scale); 
		model = glm::translate(model, localOffset.pos);
		model = glm::rotate(model, glm::radians(localOffset.angle), localOffset.rotationAxis);
		// world pos
		model = glm::scale(model, transform.scale);
		model = glm::translate(model, transform.pos);
		model = glm::rotate(model, glm::radians(transform.angle), transform.rotationAxis);
		int modelLoc = glGetUniformLocation(shader->ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	}
public:
	Model* objModel;
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
	void Draw() {
		shader->use();
		for (Mesh* mesh : objModel->meshes) {
			for (int i = 0; i < mesh->textures.size(); i ++) {
				glActiveTexture(GL_TEXTURE0 + i);
				std::string name = "ourTexture";
				glUniform1i(glGetUniformLocation(shader->ID, name.c_str()), i);
				glBindTexture(GL_TEXTURE_2D, mesh->textures[i]->id);
			}

			if (mesh->emitter != NULL) {
				shader->setVec3("emitterColor", mesh->emitter->rgb);
			}
			else {
				shader->setVec3("emitterColor", glm::vec3(0));
			}

			genModelMatrix();

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