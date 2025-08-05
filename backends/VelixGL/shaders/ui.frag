#version 450

layout (location = 0) out vec4 FragColor;

in vec2 vUV;

uniform sampler2D uTexture;

uniform vec4 uColor;
uniform float uAlpha;
uniform bool uUseTexture;

void main()
{
    vec4 finalColor = uUseTexture ? texture(uTexture, vUV) * uColor : uColor;
    finalColor.a *= uAlpha;
    FragColor = finalColor;
}
