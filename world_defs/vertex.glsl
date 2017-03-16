#version 120

varying vec2 texture_coordinate;
varying vec4 gl_FrontColor;

void main()
{
    // Transforming The Vertex
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

    // Passing The Texture Coordinate Of Texture Unit 0 To The Fragment Shader
    texture_coordinate = vec2(gl_MultiTexCoord0);
    gl_FrontColor = gl_Color;
    gl_BackColor = gl_Color;
}