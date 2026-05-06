#include "World.h"
#include "Chunk.h" 
#include <iostream>


World::World() {}

World::~World() {
    for (auto& pair : chunks) {
        delete pair.second;
    }
}

bool World::isSolidAt(int x, int y, int z) {
    if (y < 0 || y >= chunkSize) return false;


    int cx = std::floor((float)x / (float)chunkSize);
    int cz = std::floor((float)z / (float)chunkSize);
    glm::ivec2 chunkPos(cx, cz);
    if (chunks.find(chunkPos) == chunks.end()) return false;

    int lx = x % chunkSize; if (lx < 0) lx += chunkSize;
    int lz = z % chunkSize; if (lz < 0) lz += chunkSize;

    return chunks[chunkPos]->isSolid(lx, y, lz);
}

void World::update(glm::vec3 playerPos) {
    int pX = std::floor(playerPos.x / (float)chunkSize);
    int pZ = std::floor(playerPos.z / (float)chunkSize);

    for (int x = pX - renderDistance; x <= pX + renderDistance; x++) {
        for (int z = pZ - renderDistance; z <= pZ + renderDistance; z++) {
            if (chunks.find(glm::ivec2(x, z)) == chunks.end()) {
                loadChunk(x, z);
            }
        }
    }

    for (auto const& [pos, chunk] : chunks)
    {
		if (chunk->needMeshUpdate) {
            chunk->rebuildMesh(this);
            chunk->needMeshUpdate = false;
        }
    }

}

void World::setBlock( int gx, int gy, int gz, int blockID)
{
	int chunkX = floor((float)gx / (float)chunkSize);
	int chunkZ = floor((float)gz / (float)chunkSize);

    int lx = gx - (chunkX * chunkSize);
	int lz = gz - (chunkZ * chunkSize);

	Chunk* chunk = chunks[glm::ivec2(chunkX, chunkZ)];
    if (chunk && gy >= 0 && gy < chunkHeight) {
        chunk->data[lx][gy][lz] = blockID;
        

        chunk->rebuildMesh(this);

       
    }

}

void World::loadChunk(int x, int z) {
    chunks[glm::ivec2(x, z)] = new Chunk(x, z);
	//chunks[glm::ivec2(x, z)] = std::make_unique<Chunk>(x, z);
}



void World::render(unsigned int shaderProgram) {
    for (auto const& [pos, chunk] : chunks) {
        chunk->render(shaderProgram);
    }
}