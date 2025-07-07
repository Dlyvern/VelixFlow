#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aUV;

uniform mat4 uProjection;
uniform vec2 uPosition;
uniform vec2 uSize;

out vec2 vUV;

void main() {
    vec2 pos = aPos * uSize + uPosition;
    gl_Position = uProjection * vec4(pos, 0.0, 1.0);
    vUV = aUV;
}
