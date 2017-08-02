#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 projection_mat;
uniform mat4 view;
uniform mat4 model;

void main()
{
    vec4 pos = vec4(aPos, 1.0);
    gl_Position = projection_mat * view * model * pos;
}
