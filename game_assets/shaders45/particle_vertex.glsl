#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in float aSize;

out vec3 color;

layout (std140) uniform camera_data
{
    mat4 projection_matrix;
    mat4 view_matrix;
    mat4 proj_view_matrix;
    vec3 camera_position;
};

void main()
{
    color = aColor;
    gl_PointSize = aSize;
    gl_Position = projection_matrix * (view_matrix * vec4(aPos, 1.0));
}