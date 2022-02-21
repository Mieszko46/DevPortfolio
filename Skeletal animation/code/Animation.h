#pragma once

#include <vector>
#include <map>

#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <functional>

#include "Bone.h"
#include "BoneInfo.h"
#include "Model.h"

struct AssimpNodeData
{
	glm::mat4 transformation;
	std::string name;
	int childrenCount;
	std::vector<AssimpNodeData> children;
};

class Animation
{
public:
	Animation() = default;
	Animation(const std::string& animationPath, Model* model);
	Bone* FindBone(const std::string& name);
	float GetTicksPerSecond();
	float GetDuration();
	const AssimpNodeData& GetRootNode();
	const std::map<std::string, BoneInfo>& GetBoneIDMap();

private:

	float m_Duration;
	int m_TicksPerSecond;
	std::vector<Bone> m_Bones;
	AssimpNodeData m_RootNode;
	std::map<std::string, BoneInfo> m_BoneInfoMap;

	void SetupBones(const aiAnimation* animation, Model& model);
	void ReadHierarchyData(AssimpNodeData& path, const aiNode* src);
};

