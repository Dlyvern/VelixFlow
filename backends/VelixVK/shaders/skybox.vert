#version 450

layout (location = 0) in vec3 aPos;

layout (location = 0) out vec3 texCoords;

layout (set = 0, binding = 0) uniform UBO
{
    mat4 projection;
    mat4 view;
} ubo;

void main()
{
    vec4 pos = ubo.projection * ubo.view * vec4(aPos, 1.0f);
    gl_Position = vec4(pos.x, pos.y, pos.w, pos.w);
    texCoords = vec3(aPos.x, aPos.y, -aPos.z);
}