#version 430

in VS_OUT {
    vec3 world_pos;
    vec3 base_normal;
    vec3 color;
    vec3 tint;
} fs_in;

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gAlbedo;
layout (location = 3) out vec3 gAmbientOcclusion;

out vec4 FragColor;

vec3 degamma(vec3 col) {
    return pow(col, vec3(2.2));
}

void main() {
    vec3 base_color = fs_in.color * fs_in.tint;
    //vec3 base_color = textureLoc;
    //if (base_color.r < 0.5) base_color.r = 0.5;
    //vec3 base_color = vec3(1.0, 0.0, 0.0);
    gAlbedo = base_color * fs_in.tint;
    gPosition = vec3(fs_in.world_pos);

    gNormal = normalize(fs_in.base_normal) * 0.5 + 0.5; // Convert to RGB space

    gAmbientOcclusion.r = 1.0; // No ambient occlusion
    gAmbientOcclusion.g = 0.04; // No metal
    gAmbientOcclusion.b = 0.25; // 50% rough
    //gAlbedo = lightColor;
}