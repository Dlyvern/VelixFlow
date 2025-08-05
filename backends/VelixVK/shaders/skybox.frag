#version 450

layout (location = 0) out vec4 FragColor;

layout (location = 0) in vec3  texCoords;

layout(set = 0, binding = 0) uniform samplerCube skybox;

void main()
{
    FragColor = texture(skybox, texCoords);
}