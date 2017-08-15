#version 330 core

uniform sampler2DArray textureArray;
uniform sampler3D world_data;
uniform sampler3D light_location;
uniform sampler3D light_color;

in vec3 tex_pos;
in vec3 world_pos;
in mat3 TBN;

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gAlbedo;
layout (location = 3) out vec3 gAmbientOcclusion;
layout (location = 4) out vec3 gLightPos;
layout (location = 5) out vec3 gLightCol;

out vec4 FragColor;

void main() {
    float gamma = 2.2;
    vec3 base_color = pow(texture(textureArray, tex_pos).rgb, vec3(gamma));
    gAlbedo = base_color;
    gPosition = vec3((world_pos.x+0.5)/256.0, (world_pos.z+0.5)/256.0, (world_pos.y+0.5)/128.0);

    vec3 norm = texture(textureArray, vec3(tex_pos.x, tex_pos.y, tex_pos.z+1)).rgb;
    norm = normalize(norm * 2.0 - 1.0);
    norm = normalize(TBN * norm);
    gNormal = norm;

    vec3 world_info = texture(world_data, gPosition).rgb;
    vec3 lightLocation = texture(light_location, gPosition).rgb;
    vec3 lightColor = texture(light_color, gPosition).rgb;

    gLightCol = world_info.r > 0.0 ? vec3(1.0) : lightColor;
    gLightPos = world_info.r > 0.0 ? vec3(40, 256, 128) / 256.0 : lightLocation / 256.0;

    gAmbientOcclusion.r = pow(texture(textureArray, vec3(tex_pos.x, tex_pos.y, tex_pos.z+2)).r, gamma);
    gAmbientOcclusion.g = pow(texture(textureArray, vec3(tex_pos.x, tex_pos.y, tex_pos.z+2)).g, gamma);
    gAmbientOcclusion.b = pow(texture(textureArray, vec3(tex_pos.x, tex_pos.y, tex_pos.z+2)).b, gamma);
    //gAlbedo = lightColor;
}