#version 450

layout (location = 0) in vec3 aPos;

layout (location = 0) out vec3 localPos;

layout (set = 0, binding = 0) uniform UBO
{
    mat4 projection;
    mat4 view;
} ubo;

void main()
{
    localPos = aPos;
    gl_Position =  ubo.projection * ubo.view * vec4(localPos, 1.0);
}
