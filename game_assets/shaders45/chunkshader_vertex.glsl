#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 texture_info;
layout (location = 2) in vec3 aNormal;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform vec3 camera_position;
uniform mat3 normalMatrix;

out VS_OUT {
    vec3 tex_pos;
    vec3 world_pos;
} vs_out;

void main()
{
    vs_out.tex_pos = texture_info;
    vs_out.world_pos = aPos;
    gl_Position = projection_matrix * view_matrix * vec4(aPos, 1.0);
}
