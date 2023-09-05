#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Transform.h"
#include "TerrainGenerator.h"
#include "UtilityFunctions.h"
#include "Vertex.h"
#include "GrassData.h"
#include "Texture.h"
#include "PerlinNoise.h"

class GrassGenerator {
private:
	int xCount;
	int zCount;
	Mesh grassMesh;
	Model grassModel;
	float posVariance, sizeVariance, rotVariance;
	int VAO;
	glm::mat4* modelMatrices = nullptr;
	glm::vec3* windMap = nullptr;
	glm::vec3* translationMap = nullptr;
	glm::vec3* scaleMap = nullptr;
	Texture* texture = nullptr;

	const siv::PerlinNoise::seed_type seed = 123456u;
	const siv::PerlinNoise perlin{ seed };

	float PerlinWind(float x, float y, float amp, float fre) {
		return perlin.noise2D(fre * x, fre * y) * amp;
	}
public:
	GrassGenerator(
		int _xCount = 3, 
		int _zCount = 3, 
		float _posV = 0.08f,
		float _sizeV = 0.2f,
		float _rotV = 1.0f,
		Texture* _texture = nullptr
	) {
		SetVars(_xCount, _zCount, _posV, _sizeV, _rotV, _texture);
		grassModel = Model(&grassMesh);
	}
	GrassGenerator(const GrassGenerator& other) {
		SetVars(
			other.xCount,
			other.zCount,
			other.posVariance,
			other.sizeVariance,
			other.rotVariance,
			other.texture
		);
		grassMesh = other.grassMesh;
		grassModel = Model(&grassMesh);
	}
	void operator= (GrassGenerator other) {
		SetVars(
			other.xCount, 
			other.zCount, 
			other.posVariance,
			other.sizeVariance,
			other.rotVariance,
			other.texture
		);
		grassMesh = other.grassMesh;
		grassModel = Model(&grassMesh);
	}
	void SetVars(
		int _xCount = 3,
		int _zCount = 3,
		float _posV = 0.08f,
		float _sizeV = 0.2f,
		float _rotV = 1.0f,
		Texture* _texture = nullptr
	) {
		xCount = _xCount;
		zCount = _zCount;
		posVariance = _posV;
		sizeVariance = _sizeV;
		rotVariance = _rotV;
		texture = _texture;

		std::vector<Vertex> grassVD;
		GrassData grassData;

		glm::vec3 grassColor(52.f, 87.f, 67.f);

		int ver = 0, norm = 0;
		for (int i = 0; i < (int)(grassData.sizeV / 3); i++) {
			ver = i * 3;
			norm = i * 3;
			grassVD.emplace_back(
				Vertex(
					grassData.vertices[ver],
					grassData.vertices[ver + 1],
					grassData.vertices[ver + 2],
					0, 0, 0,
					0, 0,
					grassColor.x / 255.f, grassColor.y / 255.f, grassColor.z / 255.f
					//144.f / 255.f, 245.f / 255.f, 66.f / 255.f
				)
			);
		}
		int ind = 0;
		int a, b, c;
		for (int i = 0; i < (int)(grassData.sizeI / 3); i++) {
			ind = i * 3;
			a = grassData.indices[ind] * 3;
			b = grassData.indices[ind + 1] * 3;
			c = grassData.indices[ind + 2] * 3;

			glm::vec3 aa(
				grassData.vertices[a],
				grassData.vertices[a + 1],
				grassData.vertices[a + 2]
			);
			glm::vec3 bb(
				grassData.vertices[b],
				grassData.vertices[b + 1],
				grassData.vertices[b + 2]
			);
			glm::vec3 cc(
				grassData.vertices[c],
				grassData.vertices[c + 1],
				grassData.vertices[c + 2]
			);
			glm::vec3 cross = CrossProduct(
				aa,
				bb,
				cc
			);
			grassVD[grassData.indices[ind]].Normal = cross;
			grassVD[grassData.indices[ind + 1]].Normal = cross;
			grassVD[grassData.indices[ind + 2]].Normal = cross;
		}
		grassMesh = Mesh(grassVD, grassData.indices, grassData.sizeI, texture);
	}

	GameObject GenerateModelMatrices(TerrainGenerator* terrain, Shader* shader) {

		int terX = terrain->xSize - 1;
		int terZ = terrain->zSize - 1;
		glm::vec2 off = terrain->chunkOffset;
		float terDist = terrain->dist;

		GameObject tempGrassModel(
			grassModel,
			shader,
			Transform(),
			Transform(),
			glm::vec3(((float)terX * terDist) * off.x, 0, ((float)terZ * terDist) * off.y),
			4,
			true,
			xCount,
			zCount
		);

		modelMatrices = new glm::mat4[(xCount * zCount)];
		translationMap = new glm::vec3[(xCount * zCount)];
		scaleMap = new glm::vec3[(xCount * zCount)];

		int i = 0;
		float worldX = 0.f, worldZ = 0.f;
		float ax = ((float)terX * terDist) / (float)(xCount - 1);
		float bx = (float)terX * terDist * (-0.5f);
		float az = ((float)terZ * terDist) / (float)(zCount - 1);
		float bz = (float)terZ * terDist * (-0.5f);

		for (int x = 0; x < xCount; x++) {
			for (int z = 0; z < zCount; z++) {
				worldX = ax * (float)x + bx;
				worldZ = az * (float)z + bz;
				Transform temp(
					glm::vec3(
						worldX + GetRand() * posVariance + ((float)terX * terDist) * off.x,
						terrain->NoiseMap(
							(float)x * ((float)(terX) / (float)(xCount - 1)),
							(float)z * ((float)(terZ) / (float)(zCount - 1))
						) + std::abs(GetRand()) * posVariance * sizeVariance, // GET HEIGHT USING THE SAME NOISE FUNCTION AS THE TERRAIN
						worldZ + GetRand() * posVariance + ((float)terZ * terDist) * off.y
					),
					glm::vec3(
						1.f + GetRand() * sizeVariance * 2,
						1.f + GetRand() * sizeVariance,
						1.f
					),
					glm::vec3(0, 1, 0),
					GetRand() * 180.f * rotVariance
				);

				modelMatrices[i] = temp.genRotateModelMatrix();
				translationMap[i] = temp.pos;
				scaleMap[i] = temp.scale;
				i++;
			}
		}
		
		unsigned int buffer;
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, xCount * zCount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);
		VAO = grassMesh.getVAO();
		glBindVertexArray(VAO);
		// vertex attributes
		std::size_t vec4Size = sizeof(glm::vec4);
		std::size_t stride = vec4Size * 4;
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, stride, (void*)0);
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, stride, (void*)(1 * vec4Size));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, stride, (void*)(2 * vec4Size));
		glEnableVertexAttribArray(7);
		glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, stride, (void*)(3 * vec4Size));

		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);
		glVertexAttribDivisor(7, 1);

		///////////////////////////////////////////////////////////
		windMap = new glm::vec3[(xCount * zCount)];
		float baseFreq = 2.f;

		i = 0;
		for (int x = 0; x < xCount; x++) {
			for (int z = 0; z < zCount; z++) {
				glm::vec3 temp(
					PerlinWind(x, z, 0.001f, 0.05f),
					baseFreq + 0.5f * PerlinWind(x, z, 0.1f, 0.05f),
					0.5f + 0.5f * perlin.normalizedOctave2D((float)x * 100.f, (float)z * 100.f, 4)
				);
				windMap[i] = temp;
				i++;
			}
		}

		std::size_t floatSize = sizeof(float);
		std::size_t vec3Size = sizeof(glm::vec3);
		stride = vec3Size;
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, (xCount * zCount * vec3Size), &windMap[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(8);
		glVertexAttribPointer(8, 1, GL_FLOAT, GL_FALSE, stride, (void*)(offsetof(glm::vec3, x)));
		glEnableVertexAttribArray(9);
		glVertexAttribPointer(9, 1, GL_FLOAT, GL_FALSE, stride, (void*)(offsetof(glm::vec3, y)));
		glEnableVertexAttribArray(10);
		glVertexAttribPointer(10, 1, GL_FLOAT, GL_FALSE, stride, (void*)(offsetof(glm::vec3, z)));

		glVertexAttribDivisor(8, 1);
		glVertexAttribDivisor(9, 1);
		glVertexAttribDivisor(10, 1);

		//////////////////////////////

		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, (xCount * zCount * vec3Size), &translationMap[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(11);
		glVertexAttribPointer(11, 3, GL_FLOAT, GL_FALSE, stride, (void*)(offsetof(glm::vec3, x)));
		glVertexAttribDivisor(11, 1);

		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, (xCount * zCount * vec3Size), &scaleMap[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(12);
		glVertexAttribPointer(12, 3, GL_FLOAT, GL_FALSE, stride, (void*)(offsetof(glm::vec3, x)));
		glVertexAttribDivisor(12, 1);

		glBindVertexArray(0);

		return tempGrassModel;
	}
	~GrassGenerator() {
		delete[] windMap;
		delete[] modelMatrices;
		delete[] translationMap;
		delete[] scaleMap;
	}
};