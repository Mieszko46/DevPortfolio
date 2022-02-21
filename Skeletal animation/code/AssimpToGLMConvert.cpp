#include "AssimpToGLMConvert.h"

glm::mat4 AssimpToGLMConvert::ConvertMatrixToGLMFormat(const aiMatrix4x4& from)
{
    glm::mat4 result;
    // the a,b,c,d in Assimp is the row; the 1,2,3,4 is the column
    result[0][0] = from.a1; result[1][0] = from.a2; result[2][0] = from.a3; result[3][0] = from.a4;
    result[0][1] = from.b1; result[1][1] = from.b2; result[2][1] = from.b3; result[3][1] = from.b4;
    result[0][2] = from.c1; result[1][2] = from.c2; result[2][2] = from.c3; result[3][2] = from.c4;
    result[0][3] = from.d1; result[1][3] = from.d2; result[2][3] = from.d3; result[3][3] = from.d4;

    return result;
}

glm::vec3 AssimpToGLMConvert::GetGLMVec(const aiVector3D& vec)
{
    return glm::vec3(vec.x, vec.y, vec.z);
}

glm::quat AssimpToGLMConvert::GetGLMQuat(const aiQuaternion& pOrientation)
{
    return glm::quat(pOrientation.w, pOrientation.x, pOrientation.y, pOrientation.z);
}
