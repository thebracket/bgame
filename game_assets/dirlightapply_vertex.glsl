#version 330 core
layout (location = 0) in vec3 aPos;

out vec4 FragPosLightSpace;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 lightSpaceMatrix;

void main()
{
    FragPosLightSpace = lightSpaceMatrix * vec4(aPos, 1.0);
    gl_Position = projection_matrix * view_matrix * vec4(aPos, 1.0);
}
