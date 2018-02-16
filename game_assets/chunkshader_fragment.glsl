#version 330 core

uniform sampler2DArray textureArray;

in vec3 tex_pos;
in vec3 world_pos;

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gAlbedo;
layout (location = 3) out vec3 gAmbientOcclusion;

out vec4 FragColor;

void main() {
    vec3 base_color = texture(textureArray, vec3(tex_pos.x, tex_pos.y, tex_pos.z)).rgb;
    gAlbedo = base_color;
    gPosition = vec3(world_pos);

    vec3 normal = texture(textureArray, vec3(tex_pos.x, tex_pos.y, tex_pos.z+1)).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    gNormal = normal;

    gAmbientOcclusion.r = texture(textureArray, vec3(tex_pos.x, tex_pos.y, tex_pos.z+2)).r;
    gAmbientOcclusion.g = texture(textureArray, vec3(tex_pos.x, tex_pos.y, tex_pos.z+2)).g;
    gAmbientOcclusion.b = texture(textureArray, vec3(tex_pos.x, tex_pos.y, tex_pos.z+2)).b;    
}