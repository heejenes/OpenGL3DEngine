#include "GrassGenerator.h"
#include "Shader.h"

#pragma once

class Chunk {
private:
	GrassGenerator grass;
	TerrainGenerator terrain;
	glm::vec2 worldOffset;
	float chunkWidth, terrainRes, grassRes;
	std::vector<GameObject>* allGO = nullptr;
	int allGOIndex = 0;
public:
	Chunk(
		glm::vec2 _worldOffset,
		float _chunkWidth,
		float _terrainRes,
		float _grassRes,
		Shader* defaultShader,
		Texture* defaultTexture
	) {
		chunkWidth = _chunkWidth;
		terrainRes = _terrainRes;
		grassRes = _grassRes;

		worldOffset = _worldOffset;

		int grassWidth = chunkWidth * grassRes;
		grass.SetVars(
			grassWidth,
			grassWidth,
			0.08f,
			0.2f, 
			1.0f, 
			defaultTexture
		);

		int terrainWidth = chunkWidth * terrainRes;
		float terrainDist = 1.f / terrainRes;
		terrain.SetVars(
			terrainWidth,
			terrainWidth,
			terrainDist,
			worldOffset,
			defaultShader,
			defaultTexture,
			defaultTexture
		);
	}
	void LoadChunk(std::vector<GameObject>* allGameObjects, Shader* grassShader) {
		allGO = allGameObjects;
		GameObject terrainObject = terrain.GetGameObject();
		GameObject grassObject = grass.GenerateModelMatrices(&terrain, grassShader);
		allGOIndex = allGameObjects->size();
		allGameObjects->push_back(terrainObject);
		allGameObjects->push_back(grassObject);
	}
	void UnloadChunk() {
		std::vector<GameObject>::iterator it = allGO->begin() + allGOIndex;
		allGO->erase(it, it + 1);
	}
};