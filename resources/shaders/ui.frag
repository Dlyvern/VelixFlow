#version 330 core
in vec2 vUV;
out vec4 FragColor;

uniform sampler2D uTexture;
uniform vec4 uColor;
uniform float uAlpha;

void main() {
    vec4 texColor = texture(uTexture, vUV);
    FragColor = texColor * uColor;
    FragColor.a *= uAlpha;
}
