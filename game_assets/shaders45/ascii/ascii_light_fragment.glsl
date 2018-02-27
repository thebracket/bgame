#version 330 core

uniform sampler2D ascii_tex;
layout (location = 1) out vec3 gAlbedo;
uniform vec3 light_position;
uniform vec3 light_color;
uniform float far_plane;
uniform samplerCube depthMap;

in VS_OUT {
    in vec3 world_pos;
    in vec2 tex_pos;
    in vec3 fg;
    in vec3 bg;
} vs_in;

// array of offset direction for sampling
vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

float shadow(vec3 fragToLight) {
    float shadow = 0.0;
    const float diskRadius = 0.06;
    const float bias = 0.15;
    const int samples = 20;
    float currentDepth = length(fragToLight);
    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(depthMap, fragToLight + gridSamplingDisk[i] * diskRadius).r;
        closestDepth *= far_plane;   // undo mapping [0;1]
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);
    shadow = 1.0 - shadow;
    return shadow;
}

void main() {
    vec3 position = vs_in.world_pos.xzy;
    vec3 fragToLight = position - light_position;
    float currentDepth = length(fragToLight);

    // Quick discard if we are out of the light's radius
    if (currentDepth > far_plane) {
        discard;
    }

    // Discard if the light can't see it
    float shadowFactor = shadow(fragToLight);
    if (shadowFactor < 0.1) {
        discard;
    }

    vec4 pixel_color = texture(ascii_tex, vs_in.tex_pos);
    vec3 out_color = pixel_color.r > 0.5 ? vs_in.fg * pixel_color.rgb : vs_in.bg;
    gAlbedo = out_color * light_color;
    //gAlbedo = vec3(shadowFactor);
}