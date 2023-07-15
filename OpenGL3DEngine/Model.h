#pragma once

class Model {
public:
	std::vector<Mesh*> meshes;

	Model(int n = 0) { meshes.resize(n); }

	Model(Mesh* _mesh) {
		meshes.push_back(_mesh);
	}
	Model(std::vector<Mesh*> _meshes) {
		meshes = _meshes;
	}
};