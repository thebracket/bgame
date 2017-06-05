#version 120

// Pass the matrices in as uniforms
uniform mat4 projection_matrix;
uniform mat4 view_matrix;

// World position is part of the VBO
attribute vec3 world_position;
attribute vec3 normal;
attribute vec3 color;
attribute vec2 texture_position;

// Outputs
varying vec3 base_normal;
varying vec3 tint;

void main() {
    vec4 position = gl_Vertex;
    position.xyz += world_position.xzy;
    gl_Position = projection_matrix * (view_matrix * position);
    base_normal = normal;
    tint = color;
    gl_TexCoord[0] = vec4(texture_position, 0.0, 0.0);
}
