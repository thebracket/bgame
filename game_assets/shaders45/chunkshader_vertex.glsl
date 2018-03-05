#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 texture_info;
layout (location = 2) in float aNormal;

layout (std140) uniform camera_data
{
	mat4 projection_matrix;
	mat4 view_matrix;
	mat4 proj_view_matrix;
	vec3 camera_position;
};

out VS_OUT {
    vec3 tex_pos;
    vec3 world_pos;
    vec3 base_normal;
} vs_out;

const vec3 normal_lut[6] = vec3[6](
        vec3(0.0, 1.0, 0.0), // Top
        vec3(0.0f,  0.0f, -1.0f), // Back
        vec3(0.0f,  0.0f, 1.0f), // Front
        vec3(-1.0f,  0.0f,  0.0f), // Left
        vec3(1.0f,  0.0f,  0.0f), // Right
        vec3(0.0f, -1.0f,  0.0f) // Bottom
    );

void main()
{
    vs_out.tex_pos = texture_info;
    vs_out.world_pos = aPos;
    vs_out.base_normal = normal_lut[int(floor(aNormal))];
    gl_Position = projection_matrix * view_matrix * vec4(aPos, 1.0);
}
