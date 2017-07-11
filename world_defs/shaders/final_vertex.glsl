#version 120

varying vec4 tex_coord;

void main() {
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    tex_coord = gl_MultiTexCoord0;

    gl_FrontColor = vec4(1.0, 1.0, 1.0, 1.0);
}
