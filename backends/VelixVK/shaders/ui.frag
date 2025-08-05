#version 450

layout (location = 0) in vec2 vUV;

layout (location = 0) out vec4 FragColor;

layout (set = 0, binding = 0) uniform sampler2D uTexture;

layout (set = 0, binding = 0) uniform UBO
{
    vec4 uColor;
    float uAlpha;
    bool uUseTexture;
} ubo;

void main()
{
    vec4 finalColor = ubo.uUseTexture ? texture(uTexture, vUV) * ubo.uColor : ubo.uColor;
    finalColor.a *= ubo.uAlpha;
    FragColor = finalColor;
}
