#version 330 core

uniform sampler2DArray textureArray;

in vec3 tex_pos;
in vec3 world_pos;
in mat3 TBN;
in vec3 base_normal;
in vec3 tint;

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gAlbedo;
layout (location = 3) out vec3 gAmbientOcclusion;

out vec4 FragColor;

void main() {
    vec4 base_color = texture(textureArray, tex_pos).rgba;
    if (base_color.a ==0) discard;
    gAlbedo = base_color.rgb * tint;
    gPosition = vec3(world_pos);

    gNormal = base_normal;

    gAmbientOcclusion = vec3(1.0);
}