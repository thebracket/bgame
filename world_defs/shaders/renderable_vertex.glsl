#version 120

// Pass the matrices in as uniforms
uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform vec3 camera_position;

// World position is part of the VBO
attribute vec3 world_position;
attribute vec3 normal;
attribute vec3 color;
attribute vec2 texture_position;
attribute vec3 flags;
attribute vec3 light_position;
attribute vec3 light_color;

// Outputs
varying vec3 tint;
varying vec3 world_pos;
varying vec4 interpolated_position;
varying vec3 flag_out;
varying vec3 light_pos;
varying vec3 light_col;
varying vec3 normal_out;

mat4 translate(float x, float y, float z){
    return mat4(
        vec4(1.0, 0.0, 0.0, 0.0),
        vec4(0.0, 1.0, 0.0, 0.0),
        vec4(0.0, 0.0, 1.0, 0.0),
        vec4(x,   y,   z,   1.0)
    );
}

void main() {
    bool billboard_mode;
    if (flags.z != 1.0) {
        billboard_mode = false;
    } else {
        billboard_mode = true;
    }

    vec4 position;
    if (billboard_mode) {
        // We want to rotate to face the camera
        mat4 ModelView = view_matrix * translate(world_position.x, world_position.z, world_position.y);

        // Column 0:
        ModelView[0][0] = 1;
        ModelView[0][1] = 0;
        ModelView[0][2] = 0;

        // Column 1:
        ModelView[1][0] = 0;
        ModelView[1][1] = 1;
        ModelView[1][2] = 0;

        // Column 2:
        ModelView[2][0] = 0;
        ModelView[2][1] = 0;
        ModelView[2][2] = 1;

        vec4 position = gl_Vertex; // At this point we're in -0.5 to 0.5 space
        gl_Position = projection_matrix * (ModelView * position);
    } else {
        position = gl_Vertex;
        position.xyz += world_position.xzy;
        gl_Position = projection_matrix * (view_matrix * position);
    }

    tint = color;
    if (world_position.z < camera_position.z) {
        float darken = (camera_position.z - world_position.z) * 0.05f;
        tint.rgb -= darken;
    }
    gl_TexCoord[0] = vec4(texture_position, 0.0, 0.0);

    world_pos = world_position;
    interpolated_position = position;

    flag_out = flags;
    light_pos = light_position;
    light_col = light_color;
    normal_out = normal;
}
