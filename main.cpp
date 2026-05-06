#include <iostream>
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Texture.h"
#include "Camera.h"
#include "Chunk.h"
#include "World.h"
#include "stb/stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const unsigned int width = 1920;
const unsigned int height = 1080;


void BlockPut(Camera& camera, World& world, bool isPlacing);

std::vector<Vertex> vertices;
std::vector<unsigned int> indices;
World myWorld;


int chunk[16][16][16];




int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, "MyCraft", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	gladLoadGL();



	Shader shaderProgram("default.vert", "default.frag");
	//Shader sunShader("sun.vert", "sun.frag");

	Camera camera(width, height, glm::vec3(20.0f, 20.0f, 20.0f));

	Texture brickTex("kup.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	brickTex.texUnit(shaderProgram, "tex0", 0);

	glEnable(GL_DEPTH_TEST);


	for (int x = 0; x <chunkSize; x++) {
		for (int y = 0; y < chunkSize; y++) {
			for (int z = 0; z < chunkSize; z++) {
				//chunk[x][y][z] = (rand() % 2); // Randomly fill the chunk with solid and non-solid blocks
				chunk[x][y][z] = 1;
				//chunk[x][0][z] = 1; // Make the bottom layer solid
			}
		}
	}




	glViewport(0, 0, width, height);


	VAO VAO1;
	VAO1.Bind();

	VBO VBO1(vertices);
	EBO EBO1(indices);

	int stride = 5 * sizeof(float);

	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, stride, (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 2, GL_FLOAT, stride, (void*)(3 * sizeof(float)));


	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	double lastClickTime = 0.0;
	double clickCooldown = 0.2; 

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.5f, 0.7f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		shaderProgram.Activate();


		camera.Inputs(window);
		camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");
		myWorld.update(camera.Position);

		for (auto const& pair : myWorld.chunks)
		{
			Chunk* chunk = pair.second;
			if (chunk->needMeshUpdate) {
				chunk->rebuildMesh(&myWorld);
			}
			chunk->render(shaderProgram.ID);
		}

		myWorld.render(shaderProgram.ID);

		double currentTime = glfwGetTime();

		if (currentTime - lastClickTime >= clickCooldown)
		{
			if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
			{
				BlockPut(camera, myWorld, false);
				lastClickTime = currentTime;

			}
			else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			{
				BlockPut(camera, myWorld, true);
				lastClickTime = currentTime;
			}	
		}

		brickTex.Bind();
		myWorld.render(shaderProgram.ID);


		VAO1.Bind();
		brickTex.Bind();



		glfwSwapBuffers(window);
		glfwPollEvents();
	}





	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	brickTex.Delete();
	shaderProgram.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void BlockPut(Camera& camera, World& world, bool isPlacing) {
	glm::vec3 rayPos = camera.Position;
	glm::vec3 rayDir = camera.Orientation;

	float stepSize = 0.1f;
	float maxReach = 5.0f;

	glm::vec3 lastEmptyPos = rayPos;

	for (float t = 0; t < maxReach; t += stepSize)
	{
		rayPos += rayDir * stepSize;

		int bX = std::floor(rayPos.x);
		int bY = std::floor(rayPos.y);
		int bZ = std::floor(rayPos.z);

		if (world.isSolidAt(bX, bY, bZ))
		{
			if (isPlacing) {
				int pX = std::floor(lastEmptyPos.x);
				int pY = std::floor(lastEmptyPos.y);
				int pZ = std::floor(lastEmptyPos.z);
				world.setBlock(pX, pY, pZ, 1);
			}
			else {
				world.setBlock(bX, bY, bZ, 0);
			}
			return;
		}
		lastEmptyPos = rayPos;
	}
}