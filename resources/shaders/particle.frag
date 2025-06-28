#version 420 core
in vec2 vUV;
out vec4 FragColor;
uniform sampler2D uTexture;
void main()
{
    vec4 tex = texture(uTexture, vUV);
    
    if (tex.a < 0.1)
        discard;

    FragColor = vec4(0.5, 0.5, 1.0, 0.5);
}
