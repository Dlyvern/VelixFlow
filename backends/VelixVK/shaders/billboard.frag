#version 450

layout (location = 0) in vec2 vUV;
layout (location = 1) out vec4 FragColor;

layout (set = 0, binding = 1) uniform sampler2D uTexture;

void main()
{
    vec4 texColor = texture(uTexture, vUV);
    
    if(texColor.a < 0.1)
        discard;

    FragColor = texColor;
}
