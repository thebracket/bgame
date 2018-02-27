#version 430 core

in vec3 color;

layout (location = 0) out vec3 gAlbedo;

out vec4 FragColor;

void main() {
    gAlbedo = color;
}