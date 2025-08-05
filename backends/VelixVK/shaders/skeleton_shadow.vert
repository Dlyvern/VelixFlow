#version 450

layout (location = 0) in vec3 pos;
layout (location = 5) in ivec4 boneIds;
layout (location = 6) in vec4 weights;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;

layout (set = 0, binding = 0) uniform UBO
{
    mat4 lightSpaceMatrix;
    mat4 model;
    mat4 finalBonesMatrices[MAX_BONES];
} ubo;

void main()
{
    mat4 boneTransform = mat4(0.0);
    for (int i = 0; i < MAX_BONE_INFLUENCE; ++i) {
        if (boneIds[i] >= 0)
            boneTransform += ubo.finalBonesMatrices[boneIds[i]] * weights[i];
    }
    if (boneTransform == mat4(0.0))
        boneTransform = mat4(1.0);

    gl_Position = ubo.lightSpaceMatrix * ubo.model * boneTransform * vec4(pos, 1.0);
}