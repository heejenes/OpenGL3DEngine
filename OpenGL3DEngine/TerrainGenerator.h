#pragma once
#include "GameObject.h"
#include "PerlinNoise.h"
#include "UtilityFunctions.h"

class TerrainGenerator {
private:
	Shader* shader;
	Transform globalTransform, localTransform;
	Mesh waterMesh, landMesh;
	Model model;
	Texture* waterTex;
	Texture* landTex;

	// NOISE SETTINGS
	float scale = 0.04f;
	float amplitude = 10.0f;
	const siv::PerlinNoise::seed_type seed = 123456u;
	const siv::PerlinNoise perlin{ seed };

	Mesh GenerateFlatMesh(Texture* tex, bool noiseOn, float opacity) {
		std::vector<Vertex> vertexData;
		std::vector<unsigned int> indices;
		float x = 0.0f, y = 0.0f, z = 0.0f;
		float nx = 0.0f, ny = 0.0f, nz = 0.0f;
		float s = 0.0f, t = 0.0f;
		float r = 0.0f, g = 0.0f, b = 0.0f;

		r = 5.f / 255.f, g = 36.f / 255.f, b = 7.f / 255.f;

		float centerXOffset = ((float)xSize - 1.f) * dist * (-0.5f) + chunkOffset.x * ((float)xSize - 1.f) * dist;
		float centerZOffset = ((float)zSize - 1.f) * dist * (-0.5f) + chunkOffset.y * ((float)zSize - 1.f) * dist;
		ny = 1.0f;
		for (int xx = 0; xx < xSize; xx++) {
			for (int zz = 0; zz < zSize; zz++) {
				x = ((float)xx) * dist;
				if (noiseOn) {
					y = NoiseMap((float)xx, (float)zz);
				}
				else { y = 0; }
				z = ((float)zz) * dist;

				s = ((float)xx / ((float)xSize - 1.0f));
				t = ((float)zz / ((float)zSize - 1.0f));

				vertexData.emplace_back(
					Vertex(
						x + centerXOffset, y, z + centerZOffset,
						nx, ny, nz,
						s, t,
						r, g, b, opacity
					)
				);
			}
		}
		indices.resize(6 * ((xSize - 1) * (zSize - 1)));
		int trisIndex = 0;
		for (int i = 0; i < (xSize - 1); i++)
		{
			for (int j = 0; j < (zSize - 1); j++)
			{
				indices[trisIndex] = i * zSize + j;
				indices[trisIndex + 1] = i * zSize + j + 1;
				indices[trisIndex + 2] = (i + 1) * zSize + j;
				indices[trisIndex + 3] = i * zSize + j + 1;
				indices[trisIndex + 4] = (i + 1) * zSize + j + 1;
				indices[trisIndex + 5] = (i + 1) * zSize + j;
				trisIndex += 6;
			}
		}

		Mesh newMesh(vertexData, indices, tex);
		return newMesh;
	}
public:
	int xSize, zSize;
	glm::vec2 chunkOffset;
	glm::vec3 center;
	float dist;
	TerrainGenerator(
		int _x = 3,
		int _z = 3,
		float _dist = 1.f,
		glm::vec2 offset = glm::vec2(0, 0),
		Shader* _shader = nullptr,
		Texture* _waterTex = nullptr,
		Texture* _landTex = nullptr
	) {
		xSize = _x;
		zSize = _z;
		dist = _dist;
		chunkOffset = offset;
		shader = _shader;
		waterTex = _waterTex;
		landTex = _landTex;
		center = glm::vec3(((float)_x * _dist) * offset.x, 0, ((float)_z * _dist) * offset.y);
		globalTransform = Transform();
		localTransform = Transform();

		model = Model(1);
	}
	void SetVars(
		int _x = 3,
		int _z = 3,
		float _dist = 1.f,
		glm::vec2 offset = glm::vec2(0, 0),
		Shader* _shader = nullptr,
		Texture* _waterTex = nullptr,
		Texture* _landTex = nullptr
	) {
		xSize = _x;
		zSize = _z;
		dist = _dist;
		chunkOffset = offset;
		center = glm::vec3(((float)_x * _dist) * offset.x, 0, ((float)_z * _dist) * offset.y);
		shader = _shader;
		waterTex = _waterTex;
		landTex = _landTex;
	}
	float NoiseMap(float a, float b) {
		return perlin.normalizedOctave2D((a + chunkOffset.x * ((float)xSize - 1.f)) * scale, (b + chunkOffset.y * ((float)zSize - 1.f)) * scale, 2) * amplitude;
	}
	void GenerateLandMesh() {
		landMesh = GenerateFlatMesh(landTex, true, 1.0f);

		model.meshes[0] = &landMesh;
	}
	void GenerateWaterMesh() {
		waterMesh = GenerateFlatMesh(waterTex, false, 0.2f);

		model.meshes[1] = &waterMesh;
	}
	GameObject GetGameObject() {
		GenerateLandMesh();
		//GenerateWaterMesh();
		return GameObject(
			&model,
			shader,
			globalTransform, 
			localTransform,
			center
		);
	}
};