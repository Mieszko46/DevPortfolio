#include "Bone.h"

Bone::Bone(const std::string& name, int ID, const aiNodeAnim* channel) : 
	m_Name(name), 
	m_ID(ID),
	m_LocalTransform(1.0f)
{
	m_CountPositions = channel->mNumPositionKeys;
	for (int positionIndex = 0; positionIndex < m_CountPositions; ++positionIndex) {
		aiVector3D aiPosition = channel->mPositionKeys[positionIndex].mValue;
		float timeStamp = channel->mPositionKeys[positionIndex].mTime;
		KeyPosition data;
		data.position = AssimpToGLMConvert::GetGLMVec(aiPosition);
		data.timeStamp = timeStamp;
		m_Positions.push_back(data);
	}

	m_CountRotations = channel->mNumRotationKeys;
	for (int rotationIndex = 0; rotationIndex < m_CountRotations; ++rotationIndex) {
		aiQuaternion aiOrientation = channel->mRotationKeys[rotationIndex].mValue;
		float timeStamp = channel->mRotationKeys[rotationIndex].mTime;
		KeyRotation data;
		data.orientation = AssimpToGLMConvert::GetGLMQuat(aiOrientation);
		data.timeStamp = timeStamp;
		m_Rotations.push_back(data);
	}

	m_CountScales = channel->mNumScalingKeys;
	for (int scaleIndex = 0; scaleIndex < m_CountScales; ++scaleIndex) {
		aiVector3D scale = channel->mScalingKeys[scaleIndex].mValue;
		float timeStamp = channel->mScalingKeys[scaleIndex].mTime;
		KeyScale data;
		data.scale = AssimpToGLMConvert::GetGLMVec(scale);
		data.timeStamp = timeStamp;
		m_Scales.push_back(data);
	}
}

void Bone::Update(float animationTime)
{
	glm::mat4 translation = InterpolatePosition(animationTime);
	glm::mat4 rotation = InterpolateRotation(animationTime);
	glm::mat4 scale = InterpolateScale(animationTime);
	m_LocalTransform = translation * rotation * scale;
}

int Bone::GetPositionIndex(float animationTime)
{
	for (int index = 0; index < m_CountPositions - 1; ++index)
		if (animationTime < m_Positions[index + 1].timeStamp)
			return index;
	assert(0);
}

int Bone::GetRotationIndex(float animationTime)
{
	for (int index = 0; index < m_CountRotations - 1; ++index)
		if (animationTime < m_Rotations[index + 1].timeStamp)
			return index;
	assert(0);
}

int Bone::GetScaleIndex(float animationTime)
{
	for (int index = 0; index < m_CountScales - 1; ++index)
		if (animationTime < m_Scales[index + 1].timeStamp)
			return index;
	assert(0);
}

int Bone::GetBoneID()
{
	return m_ID;
}

float Bone::GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime)
{
	float scaleFactor = 0.0f;
	float midWayLength = animationTime - lastTimeStamp;
	float framesDiff = nextTimeStamp - lastTimeStamp;
	scaleFactor = midWayLength / framesDiff;
	return scaleFactor;
}

std::string Bone::GetBoneName() const
{
	return m_Name;
}

glm::mat4 Bone::GetLocalTransform()
{
	return m_LocalTransform;
}

glm::mat4 Bone::InterpolatePosition(float animationTime)
{
	if (m_CountPositions == 1)
		return glm::translate(glm::mat4(1.0f), m_Positions[0].position);

	int pointZeroIndex = GetPositionIndex(animationTime);
	int pointOneIndex = pointZeroIndex + 1;
	float scaleFactor = GetScaleFactor(m_Positions[pointZeroIndex].timeStamp, m_Positions[pointOneIndex].timeStamp, animationTime);
	glm::vec3 finalPosition = glm::mix(m_Positions[pointZeroIndex].position, m_Positions[pointOneIndex].position, scaleFactor);

	return glm::translate(glm::mat4(1.0f), finalPosition);
}

glm::mat4 Bone::InterpolateRotation(float animationTime)
{
	if (m_CountRotations == 1) {
		auto rotation = glm::normalize(m_Rotations[0].orientation);
		return glm::toMat4(rotation);
	}

	int pointZeroIndex = GetPositionIndex(animationTime);
	int pointOneIndex = pointZeroIndex + 1;
	float scaleFactor = GetScaleFactor(m_Rotations[pointZeroIndex].timeStamp, m_Rotations[pointOneIndex].timeStamp, animationTime);
	glm::quat finalRotation = glm::slerp(m_Rotations[pointZeroIndex].orientation, m_Rotations[pointOneIndex].orientation, scaleFactor);
	finalRotation = glm::normalize(finalRotation);

	return glm::toMat4(finalRotation);
}

glm::mat4 Bone::InterpolateScale(float animationTime)
{
	if (m_CountScales == 1) 
		return glm::scale(glm::mat4(1.0f), m_Scales[0].scale);

	int pointZeroIndex = GetPositionIndex(animationTime);
	int pointOneIndex = pointZeroIndex + 1;
	float scaleFactor = GetScaleFactor(m_Scales[pointZeroIndex].timeStamp, m_Scales[pointOneIndex].timeStamp, animationTime);
	glm::vec3 finalScale = glm::mix(m_Scales[pointZeroIndex].scale, m_Scales[pointOneIndex].scale, scaleFactor);

	return glm::scale(glm::mat4(1.0f), finalScale);

}
