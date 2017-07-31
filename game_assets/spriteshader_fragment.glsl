#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
uniform sampler2D spriteTexture;
uniform float darken;

void main()
{
    FragColor = texture(spriteTexture, TexCoord) * vec4(darken, darken, darken, 1.0);
}
