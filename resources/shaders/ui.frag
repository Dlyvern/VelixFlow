#version 420 core
in vec2 vUV;
out vec4 FragColor;

uniform sampler2D uTexture;
uniform vec4 uColor;
uniform bool hasTexture;

void main()
{
    vec4 texColor = hasTexture ? texture(uTexture, vUV) : vec4(1.0);
    if (hasTexture && texColor.a < 0.1)
        discard;
    FragColor = texColor * uColor;
}
