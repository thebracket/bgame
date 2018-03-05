#version 430 core
layout (location = 0) out vec3 FragColor;
layout (location = 1) out vec3 BrightColor;
layout (location = 2) out vec3 Shininess;

in vec2 TexCoords;

uniform sampler2D albedo_tex;
uniform sampler2D normal_tex;
uniform sampler2D position_tex;
uniform sampler2D ao_tex;
uniform sampler2D gbuffer_depth_tex;

uniform vec3 camera_position;
uniform vec3 moon_color;

uniform mat4 projection;
uniform vec3 samples[64];
uniform float useSSAO;

#define PI 3.1415926

vec3 degamma(vec3 col) {
    return pow(col, vec3(2.2));
}

void main()
{
    vec3 base_color = degamma(texture(albedo_tex, TexCoords).rgb);
    if (base_color.r == 0.0 && base_color.g == 0.0 && base_color.b == 0) discard;
    vec3 normal = normalize(texture(normal_tex, TexCoords).rgb * 2.0 - 1.0);
    vec3 position = texture(position_tex, TexCoords).rgb ;

    // Material definitions
    vec3 material_lookup = texture(ao_tex, TexCoords).rgb;
    float ambient_occlusion = material_lookup.r;
    float roughness = material_lookup.b;

    // Use https://learnopengl.com/code_viewer_gh.php?code=src/6.pbr/1.2.lighting_textured/1.2.pbr.fs
    vec3 albedo = degamma(texture(albedo_tex, TexCoords).rgb);

    // Output a brightness for blue purposes (bloom)
    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0) BrightColor = FragColor.rgb;

    // Output a shininess, eventually used for reflection
    Shininess = vec3(1.0 - roughness);

    // SSAO & Ambient
    vec3 ambient_color = albedo;
    float SSAO = 1.0;
    if (useSSAO > 0) {
        const float kernelSize = 16.0;
        const float radius = 0.02;

        vec3 FragPos = vec3(TexCoords.x, TexCoords.y, texture(gbuffer_depth_tex, TexCoords).r);
        float occlusion = 0;
        for (int i=0; i<kernelSize; ++i) {
            vec2 mysample = FragPos.xy + (samples[i].xy * radius);
            float sampleDepth = texture(gbuffer_depth_tex, mysample).r;
            vec3 sampleColor = texture(albedo_tex, mysample).rgb;
            occlusion += sampleDepth <= FragPos.z ? 0.75 : 0.0;
            ambient_color += sampleDepth <= FragPos.z ? sampleColor : albedo;
        }
        SSAO = clamp(1.0 - (occlusion / kernelSize), 0.0, 1.0);    
        ambient_color /= kernelSize+5.0;
    }
    ambient_color *= moon_color;

    float y_difference = 1.0 - ((camera_position.y - position.y) * 0.1);

    // Final color
    vec3 ambient = ambient_color * ambient_occlusion;
    FragColor = ambient; // Remove Lo because we're just doing ambient
    FragColor *= SSAO;
    FragColor *= y_difference;
    FragColor *= 0.1;
}
