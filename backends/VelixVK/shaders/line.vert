#version 450
layout (location = 0) in vec3 aPos;

layout (set = 0, binding = 0) uniform UBO
{
    mat4 view;
    mat4 projection;
} ubo;

void main()
{
    gl_Position = ubo.projection * ubo.view * vec4(aPos, 1.0);
}