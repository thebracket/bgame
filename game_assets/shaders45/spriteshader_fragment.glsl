#version 430 core
out vec4 FragColor;

in vec2 TexCoord;
uniform sampler2D spriteTexture;
uniform float darken;

void main()
{
    vec4 texturedPixel = texture(spriteTexture, TexCoord);
    if (texturedPixel.a >0.9) {
        FragColor = texturedPixel * vec4(darken, darken, darken, 1.0);
    } else {
        discard;
    }
}
