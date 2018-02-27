#version 430 core

layout (location = 0) out vec4 FragColor;

in vec3 tex_pos;
uniform sampler2DArray textureArray;

void main()
{
    FragColor = vec4(texture(textureArray, tex_pos).rgb, 0.5);
}
