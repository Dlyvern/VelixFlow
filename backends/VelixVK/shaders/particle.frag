#version 450

layout(location = 0) in vec2 vUV;

layout(location = 0) out vec4 FragColor;

layout(set = 0, binding = 0) uniform sampler2D uTexture;

void main()
{
    vec4 tex = texture(uTexture, vUV);
    
    if (tex.a < 0.1)
        discard;

    FragColor = vec4(0.5, 0.5, 1.0, 0.5);
}
