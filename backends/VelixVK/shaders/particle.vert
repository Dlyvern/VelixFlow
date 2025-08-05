#version 420 core

layout (location = 0) in vec2 aQuadVertex;
layout (location = 1) in vec3 aInstancePos;
layout (location = 2) in vec3 aVelocity;

layout (location = 0) out vec2 vUV;

layout(set = 0, binding = 0) uniform UBO
{
    mat4 uViewProj;
} ubo;


void main()
{
    float speed = length(aVelocity);
    vec2 size = vec2(0.02, clamp(speed * 0.05, 0.2, 1.0));

    vec3 worldPos = aInstancePos + vec3(aQuadVertex * size, 0.0);

    vUV = aQuadVertex * 0.5 + 0.5;

    gl_Position = ubo.uViewProj * vec4(worldPos, 1.0);
}