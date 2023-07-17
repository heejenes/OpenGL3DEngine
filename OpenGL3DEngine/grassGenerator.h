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

class GrassGenerator {
private:
	int xCount;
	int zCount;
	Mesh grassMesh;
	Model grassModel;
	float posVariance, sizeVariance, rotVariance;
	int VAO;
	glm::mat4* modelMatrices;
public:
	GrassGenerator(
		int _xCount, 
		int _zCount, 
		float _posV, 
		float _sizeV,
		float _rotV,
		Texture* texture
	) {
		xCount = _xCount;
		zCount = _zCount;
		posVariance = _posV;
		sizeVariance = _sizeV;
		rotVariance = _rotV;

		std::vector<Vertex> grassVD;
		GrassData grassData;

		int ver = 0, norm = 0;
		for (int i = 0; i < 17; i++) {
			ver = i * 3;
			norm = i * 3;
			grassVD.emplace_back(
				Vertex(
					grassData.vertices[ver],
					grassData.vertices[ver + 1],
					grassData.vertices[ver + 2],
					0, 0, 0,
					0, 0,
					144, 245, 66
				)
			);
		}
		ver = 0;
		int a, b, c;
		for (int i = 0; i < 30; i++) {
			ver = i * 3;
			a = grassData.indices[ver] * 3;
			b = grassData.indices[ver + 1] * 3;
			c = grassData.indices[ver + 2] * 3;

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
			grassVD[grassData.indices[ver]].Normal = cross;
			grassVD[grassData.indices[ver + 1]].Normal = cross;
			grassVD[grassData.indices[ver + 2]].Normal = cross;
		}
		grassMesh = Mesh(grassVD, grassData.indices, grassData.sizeI, texture);
	}
	GameObject GenerateModelMatrices(TerrainGenerator terrain, Shader* shader) {
		grassModel = Model(&grassMesh);
		GameObject tempGrassModel(
			&grassModel,
			shader,
			Transform(),
			Transform(),
			4,
			true,
			xCount,
			zCount
		);

		int terX = terrain.xSize;
		int terZ = terrain.zSize;
		float terDist = terrain.dist;

		modelMatrices = new glm::mat4[(xCount * zCount)];
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
						worldX + GetRand() * posVariance,
						terrain.NoiseMap(
							(float)x * ((float)(terX - 1) / (float)(xCount - 1)),
							(float)z * ((float)(terZ - 1) / (float)(zCount - 1))
						) + std::abs(GetRand()) * posVariance * sizeVariance, // GET HEIGHT USING THE SAME NOISE FUNCTION AS THE TERRAIN
						worldZ + GetRand() * posVariance
					),
					glm::vec3(
						1.f + GetRand() * sizeVariance,
						1.f + GetRand() * sizeVariance,
						1.f + GetRand() * sizeVariance
					),
					glm::vec3(0, 1, 0),
					GetRand() * 180.f * rotVariance
				);

				modelMatrices[i] = temp.genModelMatrix();
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
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
		glEnableVertexAttribArray(7);
		glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);
		glVertexAttribDivisor(7, 1);

		glBindVertexArray(0);
		return tempGrassModel;
	}
};