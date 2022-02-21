#define STB_IMAGE_IMPLEMENTATION
#include "Animator.h"

Animator::Animator(Animation* currentFrame)
{
	m_DeltaTime = 0.0;
	m_CurrentAnimation = currentFrame;
	m_CurrentFrameTime = 0.0;
	m_Transforms.reserve(100);
	for (int i = 0; i < 100; i++)
		m_Transforms.push_back(glm::mat4(1.0f));
}

void Animator::CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform)
{
	std::string nodeName = node->name;
	glm::mat4 nodeTransform = node->transformation;

	Bone* bone = m_CurrentAnimation->FindBone(nodeName);
	if (bone) {
		bone->Update(m_CurrentFrameTime);
		nodeTransform = bone->GetLocalTransform();
	}

	glm::mat4 globalTransformation = parentTransform * nodeTransform;

	auto boneInfoMap = m_CurrentAnimation->GetBoneIDMap();
	if (boneInfoMap.find(nodeName) != boneInfoMap.end()) {
		int index = boneInfoMap[nodeName].id;
		glm::mat4 offset = boneInfoMap[nodeName].offset;
		m_Transforms[index] = globalTransformation * offset;
	}

	for (int i = 0; i < node->childrenCount; i++)
		CalculateBoneTransform(&node->children[i], globalTransformation);
}

void Animator::UpdateAnimation(float deltaTime)
{
	m_DeltaTime = deltaTime;
	if (m_CurrentAnimation) {
		m_CurrentFrameTime += m_CurrentAnimation->GetTicksPerSecond() * deltaTime;
		m_CurrentFrameTime = fmod(m_CurrentFrameTime, m_CurrentAnimation->GetDuration());
		CalculateBoneTransform(&m_CurrentAnimation->GetRootNode(), glm::mat4(1.0f));
	}
}

std::vector<glm::mat4> Animator::GetPoseTransforms()
{
	return m_Transforms;
}
