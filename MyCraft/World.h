#ifndef WORLD_H
#define WORLD_H

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp> 
#include <unordered_map>
#include <vector>

class Chunk;

class World {
public:
    World();
    ~World();

    std::unordered_map<glm::ivec2, Chunk*> chunks;
	//std::unordered_map<glm::ivec2, std::unique_ptr<Chunk>> chunks;


    int renderDistance = 8;

   

    void update(glm::vec3 playerPos);
    void render(unsigned int shaderProgram);

	void setBlock(int x, int y, int z, int blockID);
    

    bool isSolidAt(int x, int y, int z);

private:
    void loadChunk(int x, int z);
    void unloadFarChunks(glm::vec3 playerPos);
};

#endif