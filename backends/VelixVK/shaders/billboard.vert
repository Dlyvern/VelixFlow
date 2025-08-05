#version 450

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;

layout (location = 0) out vec2 vUV;

layout(set = 0, binding = 0) uniform BillboardUniforms{
    mat4 uMVP;
} ubo;

void main()
{
    vUV = aUV;
    gl_Position = ubo.uMVP * vec4(aPos, 1.0);
}