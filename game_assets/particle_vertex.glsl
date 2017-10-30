#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in float aSize;

out vec3 color;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;

void main()
{
    color = aColor;
    gl_PointSize = 10.0;
    gl_Position = projection_matrix * (view_matrix * vec4(aPos, 1.0));
}