#pragma once
#include "GameObject.h"

class TerrainGenerator {
private:
	int xSize, zSize;
	float dist;
	Shader* shader;
	Transform globalTransform, localTransform;
	Mesh waterMesh, landMesh;
	Model model;
	Texture texture;

	Mesh GenerateFlatMesh() {
		std::vector<Vertex> vertexData;
		std::vector<unsigned int> indices;
		int tex, norm;
		float x, y, z;
		float nx, ny, nz;
		float s, t;
		float r, g, b;
		for (int i = 0; i < (xSize * zSize); i++) {
			

			vertexData.emplace_back(
				Vertex(
					x, y, z,
					nx, ny, nz,
					s, t,
					r, g, b
				)
			);
		}
	}
public:
	TerrainGenerator(
		int _x,
		int _z,
		int _dist,
		Shader* _shader, 
		Transform _transform = Transform(),
		Transform _localOffset = Transform()
	) {
		xSize = _x;
		zSize = _z;
		dist = _dist;
		shader = _shader;
		globalTransform = _transform;
		localTransform = _localOffset;
		model = Model(2);
		texture = Texture();
		GenerateLandMesh();
		GenerateWaterMesh();
	}
	void GenerateLandMesh() {
		
		

		model.meshes[0] = &landMesh;
	}
	void GenerateWaterMesh() {
		

		model.meshes[1] = &waterMesh;
	}
	GameObject GetGameObject() {

		return GameObject(
			&model,
			shader,
			globalTransform, 
			localTransform
		);
	}
};