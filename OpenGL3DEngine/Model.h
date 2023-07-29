#pragma once

class Model {
public:
	std::vector<Mesh*> meshes;

	Model(int n = 0) { 
		meshes.clear();
		meshes.resize(n); 
	}

	Model(Mesh* _mesh) {
		meshes.clear();
		meshes.push_back(_mesh);
	}
	Model(std::vector<Mesh*> _meshes) {
		meshes.clear();
		meshes.resize(_meshes.size());
		std::copy(_meshes.begin(), _meshes.end(), meshes.begin());
	}
	Model(const Model &other) {
		meshes.clear();
		meshes.resize(other.meshes.size());
		std::copy(other.meshes.begin(), other.meshes.end(), meshes.begin());
	}
	void operator= (Model other) {
		meshes.clear();
		meshes.resize(other.meshes.size());
		std::copy(other.meshes.begin(), other.meshes.end(), meshes.begin());
	}
};