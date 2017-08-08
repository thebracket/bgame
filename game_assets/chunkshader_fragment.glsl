#version 330 core

uniform sampler2DArray textureArray;

in vec3 tex_pos;
out vec4 FragColor;

void main() {
    vec3 base_color = texture(textureArray, tex_pos).rgb;
    FragColor = vec4(base_color, 1.0);
}