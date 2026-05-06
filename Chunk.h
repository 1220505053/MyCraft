#ifndef CHUNK_H
#define CHUNK_H

#include "VBO.h"
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

constexpr int chunkSize = 16;
constexpr int chunkHeight = 64;

enum FaceDirection {
    Front,
    Back,
    Left,
    Right,
    Top,
    Bottom
};



class World; // Forward declaration to avoid circular 

class Chunk {
public:
    int worldX, worldZ;
    unsigned char data[chunkSize][chunkHeight][chunkSize]; // 0 = Air, 1 = Stone, etc.

    unsigned int vao, vbo, ebo;
    int indexCount = 0;

    Chunk(int x, int z);
    ~Chunk();

    void generateTerrain();
    void rebuildMesh(World* world);
    void render(unsigned int shaderProgram);


    bool isSolid(int x, int y, int z);
    void addFace(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, int x, int y, int z, FaceDirection dir);

    bool needMeshUpdate = true;

};


#endif