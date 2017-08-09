#version 330 core

uniform sampler2DArray textureArray;

in vec3 tex_pos;
in vec3 world_pos;

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gAlbedo;

out vec4 FragColor;

void main() {
    vec3 base_color = texture(textureArray, tex_pos).rgb;
    gAlbedo = base_color;
    gPosition = vec3(world_pos.x/256.0, world_pos.y/256.0, world_pos.z/256.0);
}