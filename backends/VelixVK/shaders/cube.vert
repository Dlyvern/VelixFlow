#version 450

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

#define MAX_LIGHTS 4

layout (location = 0) out vec3 FragPos;
layout (location = 1) out vec3 Normal;
layout (location = 2) out vec2 TexCoords;
layout (location = 3) out vec4 FragPosLightSpace[MAX_LIGHTS];

layout (set = 0, binding = 0) uniform UBO
{
    mat4 model;
    mat4 view;
    mat4 projection;
    mat4 lightSpaceMatrices[MAX_LIGHTS];
} ubo;

void main()
{
    vec4 worldPosition = ubo.model * vec4(aPos, 1.0);
    FragPos = worldPosition.xyz;
    Normal = mat3(transpose(inverse(ubo.model))) * aNormal;
    TexCoords = aTexCoords;

    for (int i = 0; i < MAX_LIGHTS; ++i)
        FragPosLightSpace[i] = ubo.lightSpaceMatrices[i] * worldPosition;

    gl_Position = ubo.projection * ubo.view * worldPosition;
}