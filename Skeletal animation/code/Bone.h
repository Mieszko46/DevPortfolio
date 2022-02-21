#pragma once

#include <vector>
#include <list>

#include <assimp/scene.h>

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "AssimpToGLMConvert.h"

struct KeyPosition
{
	glm::vec3 position;
	float timeStamp;
};

struct KeyRotation
{
	glm::quat orientation;
	float timeStamp;
};

struct KeyScale
{
	glm::vec3 scale;
	float timeStamp;
};

// Container for bone data
class Bone
{
public:
	Bone(const std::string& name, int ID, const aiNodeAnim* channel);
	void Update(float animationTime);
	int GetPositionIndex(float animationTime);
	int GetRotationIndex(float animationTime);
	int GetScaleIndex(float animationTime);
	int GetBoneID();
	std::string GetBoneName() const;
	glm::mat4 GetLocalTransform();

private:
	// ----- VARIABLES -----
	// data storage
	std::vector<KeyPosition> m_Positions;
	std::vector<KeyRotation> m_Rotations;
	std::vector<KeyScale> m_Scales;
	// conters
	int m_CountPositions;
	int m_CountRotations;
	int m_CountScales;
	// single bone data
	glm::mat4 m_LocalTransform;
	std::string m_Name;
	int m_ID;

	// ----- METHODS -----
	float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);
	glm::mat4 InterpolatePosition(float animationTime);
	glm::mat4 InterpolateRotation(float animationTime);
	glm::mat4 InterpolateScale(float animationTime);
};

