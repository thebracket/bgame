#version 330 core

in vec3 world_pos;
in vec3 base_normal;
in vec3 tex_pos;
in vec3 color;

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gAlbedo;
layout (location = 3) out vec3 gAmbientOcclusion;

out vec4 FragColor;

void main() {
    vec3 base_color = color;
    //vec3 base_color = textureLoc;
    //if (base_color.r < 0.5) base_color.r = 0.5;
    //vec3 base_color = vec3(1.0, 0.0, 0.0);
    gAlbedo = base_color;
    gPosition = vec3(world_pos);

    gNormal = base_normal;

    gAmbientOcclusion.r = 0.0; // No ambient occlusion
    gAmbientOcclusion.g = 0.0; // No metal
    gAmbientOcclusion.b = 0.5; // 50% rough
    //gAlbedo = lightColor;
}