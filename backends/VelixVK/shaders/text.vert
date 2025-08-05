#version 450

layout (location = 0) in vec4 vertex;

layout (location = 0) out vec2 TexCoords;

layout (set = 0, binding = 0) uniform UBO
{
    mat4 projection;
} ubo;

void main()
{
    gl_Position = ubo.projection * vec4(vertex.xy, 0.0, 1.0);
    TexCoords = vertex.zw;
}