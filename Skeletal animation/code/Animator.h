#pragma once

#include <map>
#include <vector>

#include <glm/glm.hpp>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>

#include "Animation.h"
#include "Bone.h"

class Animator
{
public:
	Animator(Animation* currentFrame);
	void CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform);
	void UpdateAnimation(float deltaTime);
	std::vector<glm::mat4> GetPoseTransforms();

private:
	float m_CurrentFrameTime;
	float m_DeltaTime;
	std::vector<glm::mat4> m_Transforms;
	Animation* m_CurrentAnimation;
};

