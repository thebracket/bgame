#version 330 core
layout (location = 0) out vec3 FragColor;

in vec2 TexCoords;

uniform vec3 light_position;
uniform float far_plane;
uniform sampler2D world_position;
uniform samplerCube depthMap;

// array of offset direction for sampling
vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

void main()
{
    vec3 world_pos = texture(world_position, TexCoords).rgb;
    vec3 fragToLight = world_pos - light_position;

    float DST = length(fragToLight);
    if (DST > far_plane) {
        discard;
    }

    float shadow = 0.0;
    const float diskRadius = 0.1;
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
    if (shadow < 0.1) {
        discard;
    }

    FragColor = vec3(1.0);
}
