#version 430 core

uniform sampler2DArray textureArray;

in VS_OUT {
    in vec3 tex_pos;
    in vec3 world_pos;
} fs_in;

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gAlbedo;
layout (location = 3) out vec3 gAmbientOcclusion;

out vec4 FragColor;

void main() {
    vec3 base_color = texture(textureArray, vec3(fs_in.tex_pos.x, fs_in.tex_pos.y, fs_in.tex_pos.z)).rgb;
    gAlbedo = base_color;
    gPosition = vec3(fs_in.world_pos);

    vec3 normal = texture(textureArray, vec3(fs_in.tex_pos.x, fs_in.tex_pos.y, fs_in.tex_pos.z+1)).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    gNormal = normal * 0.5 + 0.5;

    gAmbientOcclusion.r = texture(textureArray, vec3(fs_in.tex_pos.x, fs_in.tex_pos.y, fs_in.tex_pos.z+2)).r;
    gAmbientOcclusion.g = texture(textureArray, vec3(fs_in.tex_pos.x, fs_in.tex_pos.y, fs_in.tex_pos.z+2)).g;
    gAmbientOcclusion.b = texture(textureArray, vec3(fs_in.tex_pos.x, fs_in.tex_pos.y, fs_in.tex_pos.z+2)).b;    
}