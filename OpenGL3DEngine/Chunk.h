#include "GrassGenerator.h"
#include "Shader.h"

#pragma once

//class Chunk {
//private:
//	GrassGenerator grass;
//	TerrainGenerator terrain;
//	glm::vec2 worldOffset;
//public:
//	Chunk(
//		glm::vec2 _worldOffset,
//		Shader* defaultShader,
//		Texture* defaultTexture
//	) {
//		worldOffset = _worldOffset;
//		grass = GrassGenerator(200, 200, 0.08f, 0.2f, 1.0f, defaultTexture);
//		terrain = TerrainGenerator(
//			200,
//			200,
//			0.2f,
//			defaultShader,
//			defaultTexture,
//			defaultTexture
//		);
//	}
//	GameObject LoadChunk(Shader* grassShader) {
//		GameObject terrainObject = terrain->GetGameObject();
//		GameObject grassObject = grass->GenerateModelMatrices(terrain, grassShader);
//		return grassObject;
//	}
//};