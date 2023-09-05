#include "Chunk.h"

#pragma once

class ChunkManager {
private:
	const int chunkWidthLimit = 25;
	float chunkDist, terrainResolution, grassResolution;
	std::vector<std::vector<bool>> loadedChunkPositions; // x is first index, y is second index
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
		loadedChunkPositions.resize(chunkWidthLimit);
		for (int i = 0; i < loadedChunkPositions.size(); i++) {
			loadedChunkPositions[i].resize(chunkWidthLimit);
			for (int j = 0; j < loadedChunkPositions[i].size(); j++) {
				loadedChunkPositions[i][j] = false;
			}
		}
	}
	void LoadNewChunks(std::vector<glm::vec2> chunkPositions) {
		for (int i = 0; i < chunkPositions.size(); i++) {
			if (chunkPositions[i].x < 0 || chunkPositions[i].x > chunkWidthLimit - 1
				|| chunkPositions[i].y < 0 || chunkPositions[i].y > chunkWidthLimit - 1) {
				continue;
			}
			if (!loadedChunkPositions[chunkPositions[i].x][chunkPositions[i].y]) {
				chunks.push_back(Chunk(
					chunkPositions[i],
					chunkDist,
					terrainResolution,
					grassResolution,
					terrainShader,
					grassShader,
					texture
				));
				loadedChunkPositions[chunkPositions[i].x][chunkPositions[i].y] = true;
			}
			//std::cout << "progress: " << (float)i / (float)chunkPositions.size() << std::endl;
		}
	}
};