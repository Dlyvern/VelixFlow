#version 450

layout (location = 0) out vec4 FragColor;

layout (set = 0, binding = 0) uniform UBO
{
    vec4 uColor;
} ubo;

void main()
{
    FragColor = ubo.uColor;
}