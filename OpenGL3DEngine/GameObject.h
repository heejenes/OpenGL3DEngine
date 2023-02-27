#pragma once

class GameObject {
private:
	void genModelMatrix() {
		glm::mat4 trans = glm::mat4(1.0f);
		// scale, translate, rotate
		// local
		trans = glm::scale(trans, localOffset.scale);
		trans = glm::translate(trans, localOffset.pos);
		trans = glm::rotate(trans, glm::radians(localOffset.angle), localOffset.rotationAxis);
		// world pos
		trans = glm::scale(trans, transform.scale);
		trans = glm::translate(trans, transform.pos);
		trans = glm::rotate(trans, glm::radians(transform.angle), transform.rotationAxis);
		int modelLoc = glGetUniformLocation(shader->ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(trans));
	}
public:
	Model* model;
	Transform transform;
	Transform localOffset;
	Shader* shader;
	int drawType;
	GameObject(char* path, Shader* _shader, Transform _transform = Transform(), Transform _localOffset = Transform(), int _drawType = GL_TRIANGLES) {
		static Model a = Model(path);
		model = &a;
		shader = _shader;
		transform = _transform;
		localOffset = _localOffset;
		drawType = _drawType;
	}
	GameObject(Model* _model, Shader* _shader, Transform _transform = Transform(), Transform _localOffset = Transform(), int _drawType = GL_TRIANGLES) {
		
		model = _model;
		shader = _shader;
		transform = _transform;
		localOffset = _localOffset;
		drawType = _drawType;
	}
	GameObject(std::vector<Mesh> _meshes, Shader* _shader, Transform _transform = Transform(), Transform _localOffset = Transform(), int _drawType = GL_TRIANGLES) {
		static Model a = Model(_meshes);
		model = &a;
		shader = _shader;
		transform = _transform;
		localOffset = _localOffset;
		drawType = _drawType;
	}
	void Draw() {
		for (Mesh mesh : model->meshes) {
			glBindVertexArray(mesh.VAO);
			for (Texture texture : mesh.textures) {
				glBindTexture(GL_TEXTURE_2D, texture.id);
			}
			std::cout << mesh.vertexData[0].Position.x << std::endl;
			genModelMatrix();
			if (mesh.usesIndex) {
				glDrawElements(drawType, mesh.indices.size(), GL_UNSIGNED_INT, 0);
			}
			else {
				glDrawArrays(drawType, 0, mesh.indices[0]);
			}
		}
	}
};