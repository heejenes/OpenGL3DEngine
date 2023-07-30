#include "GrassGenerator.h"
#include "Shader.h"

#pragma once

class Chunk {
private:
	GrassGenerator grass;
	TerrainGenerator terrain;
	glm::vec2 worldOffset;
	float chunkWidth, terrainRes, grassRes;
public:
	Shader* defaultShader = nullptr;
	Shader* grassShader = nullptr;
	Texture* defaultTexture = nullptr;
	GameObject terrainObject;
	GameObject grassObject;

	Chunk(const Chunk &_chunk) {
		chunkWidth = _chunk.chunkWidth;
		terrainRes = _chunk.terrainRes;
		grassRes = _chunk.grassRes;
		defaultShader = _chunk.defaultShader;
		grassShader = _chunk.grassShader;
		defaultTexture = _chunk.defaultTexture;
		int grassWidth = chunkWidth * grassRes;
		grass = _chunk.grass;
		terrain = _chunk.terrain;
		worldOffset = _chunk.worldOffset;
		terrainObject = terrain.GetGameObject();
		grassObject = grass.GenerateModelMatrices(&terrain, grassShader);
		//std::cout << "Copy: " << terrainObject.objModel.meshes.size() << ", " << grassObject.objModel.meshes.size() << ", offset: (" << worldOffset.x << ", " << worldOffset.y << ")" << std::endl;
	}
	Chunk(
		glm::vec2 _worldOffset,
		float _chunkWidth,
		float _terrainRes,
		float _grassRes,
		Shader* _defaultShader,
		Shader* _grassShader,
		Texture* _defaultTexture
	) {
		chunkWidth = _chunkWidth;
		terrainRes = _terrainRes;
		grassRes = _grassRes;
		defaultShader = _defaultShader;
		grassShader = _grassShader;
		defaultTexture = _defaultTexture;
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
		terrainObject = terrain.GetGameObject();
		grassObject = grass.GenerateModelMatrices(&terrain, grassShader);

		//std::cout << "Construct: " << terrainObject.objModel.meshes.size() << ", " << grassObject.objModel.meshes.size() << ", offset: (" << worldOffset.x << ", " << worldOffset.y << ")" << std::endl;
	}
};