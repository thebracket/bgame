#version 120

uniform sampler2D my_color_texture;
varying vec3 normal;

// Outdoor rendering
void main()
{
    //gl_FragColor = vec4(texture2D(my_color_texture, gl_TexCoord[0].st) * gl_Color);;
    gl_FragColor = texture2D(my_color_texture, gl_TexCoord[0].st) * gl_Color;
    //gl_FragColor = gl_Color;
}