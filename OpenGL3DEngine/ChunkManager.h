#include "Chunk.h"

#pragma once

class ChunkManager {
private:
	float chunkDist, terrainResolution, grassResolution;
	Shader* terrainShader = nullptr;
	Shader* grassShader = nullptr;
	Texture* texture = nullptr;
public:
	std::vector<Chunk> chunks;
	ChunkManager(
		float _chunkDist, 
		float _terrainResolution, 
		float _grassResolution,
		Shader* _terrainShader,
		Shader* _grassShader,
		Texture* _texture
	) {
		chunkDist = _chunkDist;
		terrainResolution = _terrainResolution;
		grassResolution = _grassResolution;
		terrainShader = _terrainShader;
		grassShader = _grassShader;
		texture = _texture;
	}
	void LoadNewChunks(std::vector<glm::vec2> chunkPositions) {
		for (int i = 0; i < chunkPositions.size(); i++) {
			chunks.push_back(Chunk(
				chunkPositions[i],
				chunkDist,
				terrainResolution,
				grassResolution,
				terrainShader,
				grassShader,
				texture
			));
		}
	}
};