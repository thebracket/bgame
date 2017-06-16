#version 120

// Pass the matrices in as uniforms
uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 light_space_matrix;
attribute vec3 world_position;

// Outputs
varying vec3 world_pos;
varying vec4 fragment_light_space_position;

void main() {
    // Very simple: we transform the object to the right world space and pass it through.
    vec4 position = gl_Vertex;
    position.xyz += world_position.xzy;
    gl_Position = projection_matrix * (view_matrix * position);
    world_pos = position.xyz;
    fragment_light_space_position = light_space_matrix * position;
}
