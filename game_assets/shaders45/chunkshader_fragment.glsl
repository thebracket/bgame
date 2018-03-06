#version 430 core

uniform sampler2DArray textureArray;

in VS_OUT {
    vec3 tex_pos;
    vec4 world_pos;
    vec3 base_normal;
    mat3 TBN;
} fs_in;

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gAlbedo;
layout (location = 3) out vec3 gAmbientOcclusion;

out vec4 FragColor;

vec3 degamma(vec3 col) {
    return pow(col, vec3(2.2));
}

void main() {
    vec3 base_color = texture(textureArray, vec3(fs_in.tex_pos.x, fs_in.tex_pos.y, fs_in.tex_pos.z)).rgb;
    gAlbedo = base_color;
    gPosition = fs_in.world_pos;

    vec3 tex_normal = texture(textureArray, vec3(fs_in.tex_pos.x, fs_in.tex_pos.y, fs_in.tex_pos.z+1)).rgb;
    tex_normal = normalize(tex_normal * 2.0 - 1.0);
    vec3 normal = normalize(fs_in.TBN * tex_normal);
    gNormal = normal;

    gAmbientOcclusion = texture(textureArray, vec3(fs_in.tex_pos.x, fs_in.tex_pos.y, fs_in.tex_pos.z+2)).rgb;
}