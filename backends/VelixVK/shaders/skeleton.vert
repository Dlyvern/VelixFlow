#version 450

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 norm;
layout(location = 2) in vec2 tex;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;
layout(location = 5) in ivec4 boneIds;
layout(location = 6) in vec4 weights;


#define MAX_LIGHTS 4
const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;


layout(set = 0, binding = 0) uniform UBO
{
    mat4 finalBonesMatrices[MAX_BONES];
    mat4 projection;
    mat4 view;
    mat4 model;
    mat4 lightSpaceMatrices[MAX_LIGHTS];
} ubo;


layout(location = 0) out vec3 FragPos;
layout(location = 1) out vec3 Normal;
layout(location = 2) out vec2 TexCoords;
layout(location = 3) out vec4 FragPosLightSpace[MAX_LIGHTS];

void main()
{
    mat4 boneTransform = mat4(0.0);
    for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
    {
        if (boneIds[i] >= 0)
            boneTransform += ubo.finalBonesMatrices[boneIds[i]] * weights[i];
    }

    if (boneTransform == mat4(0.0))
    boneTransform = mat4(1.0);

    vec4 worldPos = ubo.model * boneTransform * vec4(pos, 1.0);
    FragPos = vec3(worldPos);
    Normal = mat3(transpose(inverse(ubo.model * boneTransform))) * norm;
    TexCoords = tex;
    
    for (int i = 0; i < MAX_LIGHTS; ++i)
        FragPosLightSpace[i] = ubo.lightSpaceMatrices[i] * worldPos;

    gl_Position = ubo.projection * ubo.view * worldPos;
}
