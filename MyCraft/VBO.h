#ifndef VBO_H
#define VBO_H

#include <glad/glad.h>
#include <vector>

struct Vertex
{
	GLfloat position[3];	//GLfloat x, y, z;	// Position
	GLfloat texCoords[2];	//GLfloat r, g, b;	// Color
	//GLfloat u, v;		// Texture coordinates
};

class VBO
{
public:
	GLuint ID;
	VBO(const std::vector<Vertex>& vertices);
	void Bind();
	void Unbind();
	void Delete();
};

#endif // !VBO_H
