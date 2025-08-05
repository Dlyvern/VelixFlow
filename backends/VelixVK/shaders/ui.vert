#version 450

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aUV;

layout(location = 0) out vec2 vUV;

layout(set = 0, binding = 0) uniform UIUniforms{
    mat4 uProjection;
    vec2 uPosition;
    vec2 uSize;
} ubo;

void main() {
    vec2 pos = aPos * ubo.uSize + ubo.uPosition;
    gl_Position = ubo.uProjection * vec4(aPos * ubo.uSize + ubo.uPosition, 0.0, 1.0);
    vUV = aUV;
}
