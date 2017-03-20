#version 120

uniform sampler2D my_color_texture;

void main()
{
    // Sampling The Texture And Passing It To The Frame Buffer
    gl_FragColor = texture2D(my_color_texture, gl_TexCoord[0].xy) * gl_Color;
}