#version 450

layout (location = 0) in vec2 TexCoords;

layout (location = 0) out vec4 color;

layout(set = 0, binding = 0) uniform sampler2D text;


layout(set = 0, binding = 1) uniform UBO
{
    vec3 textColor;
} ubo;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    color = vec4(ubo.textColor, 1.0) * sampled;
}