#pragma once

#include <vector>
#include <string>

#include <gl/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"

// how many nodes can be in bones tree hierarchy 
#define MAX_BONE_INFLUENCE 4

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;

	// bone indexes which will influence this vertex
	int boneIDs[MAX_BONE_INFLUENCE];

	// weights from each bone
	float weights[MAX_BONE_INFLUENCE];
};

struct Texture {
	unsigned int ID;
	// diffuse, specular...
	std::string type;
	std::string path;
};

class Mesh
{
public:
	// mesh data
	std::vector<Vertex> m_Vertices;
	std::vector<unsigned int> m_Indices;
	std::vector<Texture> m_Textures;
	unsigned int m_VAO;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	// render mesh
	void Draw(Shader& shader);

private:
	// render data
	unsigned int m_VBO, m_EBO;
	// initializes all the buffer objects/arrays
	void SetupMesh();
};

