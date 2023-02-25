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
	Mesh* mesh;
	Transform transform;
	Transform localOffset;
	Shader* shader;
	Texture* texture;
	int drawType;
	GameObject(Mesh* _mesh, Shader* _shader, Texture* _texture, Transform _transform = Transform(), Transform _localOffset = Transform(), int _drawType = GL_TRIANGLES) {
		mesh = _mesh;
		texture = _texture;
		shader = _shader;
		transform = _transform;
		localOffset = _localOffset;
		drawType = _drawType;
	}
	void Draw() {
		glBindVertexArray(mesh->VAO);
		glBindTexture(GL_TEXTURE_2D, texture->id);

		genModelMatrix();
		if (mesh->usesIndex) {
			glDrawElements(drawType, mesh->indices.size(), GL_UNSIGNED_INT, 0);
		}
		else {
			glDrawArrays(drawType, 0, mesh->indices[0]);
		}
	}
};