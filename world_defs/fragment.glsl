#version 120

varying vec2 texture_coordinate;
uniform sampler2D my_color_texture;

void main()
{
    // Sampling The Texture And Passing It To The Frame Buffer
    gl_FragColor = texture2D(my_color_texture, texture_coordinate) * gl_Color;
}