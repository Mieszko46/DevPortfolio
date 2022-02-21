#pragma once
#include <gl/glew.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "stb_image.h"
#include "Mesh.h"
#include "Shader.h"
#include "AssimpToGLMConvert.h"
#include "BoneInfo.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#define MAX_BONE_WEIGHTS 4

class Model
{
public:

	// data storage
	std::vector<Texture> m_TexturesLoaded;	// stores all the textures loaded so far
	std::vector<Mesh> m_Meshes;
	// bones storage
	std::map<std::string, BoneInfo> m_BoneInfoMap;
	int m_BoneCounter;
	// path setup
	std::string m_Directory; // saves the folder where is model and textures
	bool m_GammaCorrection;

	Model(std::string const& path, bool gamma = false);
	// render the model
	void Draw(Shader& shader);
	auto& GetBoneOffsetMatMap() { return m_BoneOffsetMatMap; };
	int& GetBoneCount() { return m_BoneCount; };

private:

	int m_BoneCount;
	// bones offset storage
	std::map<std::string, BoneInfo> m_BoneOffsetMatMap;

	// loads a model using ASSIMP and stores resultes in the meshs vector
	void LoadModel(std::string const& path);

	// process a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if exists)
	void ProcessNode(aiNode* node, const aiScene* scene);

	// setting default bones values
	void SetVertexBoneDataToDefault(Vertex& vertex);
	void SetVertexBoneData(Vertex& vertex, int boneID, float weight);
	void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);

	// read textures for model using stb_image and send it to LoadMaterialTextures()
	unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma);

	// checks all material textures of a given type and loads the texture if they are not loaded yet
	// the required info is returned as a Texture struct
	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
};
