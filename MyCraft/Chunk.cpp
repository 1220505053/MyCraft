#include "Chunk.h"
#include "World.h"
#include <iostream>
#include "FastNoiseLite.h"

Chunk::Chunk(int x, int z) : worldX(x), worldZ(z) {
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	generateTerrain();
}

void Chunk::generateTerrain() {

	FastNoiseLite noise;
	noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	noise.SetSeed(1337);

	FastNoiseLite terrainNoise;
	terrainNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	terrainNoise.SetFrequency(0.05f);
	terrainNoise.SetFractalType(FastNoiseLite::FractalType_FBm);
	terrainNoise.SetFractalOctaves(4);

	FastNoiseLite caveNoise;
	caveNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	caveNoise.SetFrequency(0.05f);

	for (int x = 0; x < chunkSize; x++)
	{
		for (int z = 0; z < chunkSize; z++)
		{
			float gx = (float)(worldX * chunkSize + x);
			float gz = (float)(worldZ * chunkSize + z);

			float heightSample = terrainNoise.GetNoise(gx, gz);
			int surfaceHeight = (int)((heightSample + 1.0f) * 3.0f) + 2;

			for (int y = 0; y < chunkHeight; y++)
			{
				float gy = (float)y;
				if (y < surfaceHeight)
				{
					float caveSample = caveNoise.GetNoise(gx, gy, gz);

					if (caveSample > 0.4f)
					{
						data[x][y][z] = 0;
					}
					else {
						data[x][y][z] = 1;
					}
				}
				else {
					data[x][y][z] = 0;
				}

			}
		}
	}
}



	/*
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			for (int z = 0; z < 8; z++) {

				if (y < 4) data[x][y][z] = 1;
				else data[x][y][z] = 0;
			}
		}
	}
	*/

bool Chunk::isSolid(int x, int y, int z) {
	if (x < 0 || x >= chunkSize || y < 0 || y >= chunkHeight || z < 0 || z >= chunkSize) return false;
	return data[x][y][z] != 0;
}

void Chunk::addFace(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, int x, int y, int z, FaceDirection dir) {
	float fx = (float)x;
	float fy = (float)y;
	float fz = (float)z;

	unsigned int offset = (unsigned int)vertices.size();

	switch (dir) {
	case Top:

		vertices.push_back({ fx,	 1 + fy,	 1 + fz, 0, 0 });
		vertices.push_back({ fx,	 1 + fy,	 fz,	 0, 1 });
		vertices.push_back({ 1 + fx, 1 + fy,	 fz,	 1, 1 });
		vertices.push_back({ 1 + fx, 1 + fy,	 1 + fz, 1, 0 });

		indices.push_back(offset + 0);
		indices.push_back(offset + 1);
		indices.push_back(offset + 2);

		indices.push_back(offset + 0);
		indices.push_back(offset + 2);
		indices.push_back(offset + 3);

		break;
	case Bottom:
		vertices.push_back({ fx,	 fy,	 1 + fz, 0, 0 });
		vertices.push_back({ fx,	 fy,	fz,		 0, 1 });
		vertices.push_back({ 1 + fx, fy,	fz,		 1, 1 });
		vertices.push_back({ 1 + fx, fy,	1 + fz,  1, 0 });

		indices.push_back(offset + 0);
		indices.push_back(offset + 1);
		indices.push_back(offset + 2);

		indices.push_back(offset + 0);
		indices.push_back(offset + 2);
		indices.push_back(offset + 3);

		break;

	case Left:
		vertices.push_back({ fx,	 fy,	 fz,	 0, 0 });
		vertices.push_back({ fx,	 1 + fy, fz,	 0, 1 });
		vertices.push_back({ fx,	 1 + fy, 1 + fz, 1, 1 });
		vertices.push_back({ fx,	 fy,	 1 + fz, 1, 0 });

		indices.push_back(offset + 0);
		indices.push_back(offset + 1);
		indices.push_back(offset + 2);

		indices.push_back(offset + 0);
		indices.push_back(offset + 2);
		indices.push_back(offset + 3);
		break;

	case Right:
		vertices.push_back({ 1 + fx,	 fy,	 1 + fz, 0, 0 });
		vertices.push_back({ 1 + fx,	 1 + fy, 1 + fz, 0, 1 });
		vertices.push_back({ 1 + fx,	 1 + fy, fz,     1, 1 });
		vertices.push_back({ 1 + fx,	 fy,	 fz,     1, 0 });

		indices.push_back(offset + 0);
		indices.push_back(offset + 1);
		indices.push_back(offset + 2);

		indices.push_back(offset + 0);
		indices.push_back(offset + 2);
		indices.push_back(offset + 3);
		break;

	case Front:


		vertices.push_back({ fx,	 fy,	 1 + fz,  0, 0 });
		vertices.push_back({ fx,	 1 + fy, 1 + fz,  0, 1 });
		vertices.push_back({ 1 + fx, 1 + fy, 1 + fz,  1, 1 });
		vertices.push_back({ 1 + fx, fy,	 1 + fz,  1, 0 });

		indices.push_back(offset + 0);
		indices.push_back(offset + 1);
		indices.push_back(offset + 2);

		indices.push_back(offset + 0);
		indices.push_back(offset + 2);
		indices.push_back(offset + 3);

		break;

	case Back:
		vertices.push_back({ 1 + fx, fy,	 fz,	0, 0 });
		vertices.push_back({ 1 + fx, 1 + fy, fz,	0, 1 });
		vertices.push_back({ fx,	 1 + fy, fz,	1, 1 });
		vertices.push_back({ fx,	 fy,	 fz,	1, 0 });

		indices.push_back(offset + 0);
		indices.push_back(offset + 1);
		indices.push_back(offset + 2);

		indices.push_back(offset + 0);
		indices.push_back(offset + 2);
		indices.push_back(offset + 3);

		break;
	}
}

void Chunk::rebuildMesh(World* world) {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	for (int x = 0; x < chunkSize; x++) {
		for (int y = 0; y < chunkHeight; y++) {
			for (int z = 0; z < chunkSize; z++) {
				if (data[x][y][z] == 0) continue;

				// Calculate this block's GLOBAL position
				int gx = worldX * chunkSize + x;
				int gy = y;
				int gz = worldZ * chunkSize + z;

				if (!world->isSolidAt(gx, gy + 1, gz)) addFace(vertices, indices, x, y, z, Top);
				if (!world->isSolidAt(gx, gy - 1, gz)) addFace(vertices, indices, x, y, z, Bottom);
				if (!world->isSolidAt(gx + 1, gy, gz)) addFace(vertices, indices, x, y, z, Right);
				if (!world->isSolidAt(gx - 1, gy, gz)) addFace(vertices, indices, x, y, z, Left);
				if (!world->isSolidAt(gx, gy, gz + 1)) addFace(vertices, indices, x, y, z, Front);
				if (!world->isSolidAt(gx, gy, gz - 1)) addFace(vertices, indices, x, y, z, Back);
			}
		}
	}

	indexCount = indices.size();

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
	glEnableVertexAttribArray(1);

	this->indexCount = indices.size();

	std::cout << "Chunk [" << worldX << ", " << worldZ << "] generated " << indexCount << " indices (" << indexCount / 6 << " faces)" << std::endl;
}

void Chunk::render(unsigned int shaderProgram) {
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(worldX * 16, 0, worldZ * 16));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
}


Chunk::~Chunk() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
}
