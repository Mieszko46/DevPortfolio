#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNorm;
layout(location = 2) in vec2 aTex;
layout(location = 3) in ivec4 aBoneIDs; 
layout(location = 4) in vec4 aWeights;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

mat4 rotationMatrix(vec3 axis, float angle);

void main()
{
    mat4 viewModel = view * model;
    vec4 totalPosition = vec4(0.0f);
    vec3 totalNormal = vec3(0.0f);
    if(aBoneIDs[0] != -1)
    {
        for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
        {
            if(aBoneIDs[i] == -1) 
                continue;
            if(aBoneIDs[i] >= MAX_BONES) 
            {
                totalPosition = vec4(aPos,1.0f);
                break;
            }
            vec4 localPosition = finalBonesMatrices[aBoneIDs[i]] * vec4(aPos,1.0f);
            totalPosition += localPosition * aWeights[i];
            vec3 localNormal = mat3(finalBonesMatrices[aBoneIDs[i]]) * aNorm;
            totalNormal += localNormal;
        }
	
        FragPos = vec3(model * vec4(totalPosition));
        Normal = totalNormal;  
        TexCoords = aTex;
        gl_Position =  projection * viewModel * totalPosition;
    }
    else
    {
        Normal =  aNorm;  
        TexCoords = aTex;
//       vec3 temp = vec3(1.0, 0.0, 0.0);
        vec4 MeshPosition = vec4(aPos, 1.0); // * rotationMatrix(temp, -1.57);
        FragPos = vec3(model * MeshPosition);
        gl_Position = projection * viewModel * MeshPosition;
    }
}

//mat4 rotationMatrix(vec3 axis, float angle)
//{
//    axis = normalize(axis);
//    float s = sin(angle);
//    float c = cos(angle);
//    float oc = 1.0 - c;
//    
//    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
//                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
//                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
//                0.0,                                0.0,                                0.0,                                1.0);
//}