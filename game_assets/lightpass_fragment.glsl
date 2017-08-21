#version 330 core
layout (location = 0) out vec3 FragColor;
layout (location = 1) out vec3 BrightColor;

in vec2 TexCoords;

uniform sampler2D albedo_tex;
uniform sampler2D normal_tex;
uniform sampler2D position_tex;
uniform sampler2D ao_tex;
uniform sampler3D info_tex;
uniform sampler3D light_pos_tex;
uniform sampler3D light_col_tex;

uniform vec3 camera_position;

#define PI 3.1415926

vec3 lambert_diffuse(float NdL, vec3 base_color, vec3 light_color) {
    return NdL * base_color * light_color;
}

vec3 fresnel_factor(in vec3 f0, in float product)
{
    return mix(f0, vec3(1.0), pow(1.01 - product, 5.0));
}

float D_GGX(in float roughness, in float NdH)
{
    float m = roughness * roughness;
    float m2 = m * m;
    float d = (NdH * m2 - NdH) * NdH + 1.0;
    return m2 / (PI * d * d);
}

float G_schlick(in float roughness, in float NdV, in float NdL)
{
    float k = roughness * roughness * 0.5;
    float V = NdV * (1.0 - k) + k;
    float L = NdL * (1.0 - k) + k;
    return 0.25 / (V * L);
}

vec3 cooktorrance_specular(in float NdL, in float NdV, in float NdH, in vec3 specular, in float roughness)
{
    float rimLight = 1.0;

    float D = D_GGX(roughness, NdH);
    float G = G_schlick(roughness, NdV, NdL);
    float rim = mix(1.0 - roughness * rimLight * 0.9, 1.0, NdV);

    return (1.0 / rim) * specular * G * D;
}

vec3 degamma(vec3 col) {
    return pow(col, vec3(2.2));
}

void main()
{
    // TODO: Make this a uniform
    vec3 sun_position = vec3(128.0, 256.0, 128.0);
    vec3 sun_color = vec3(1.0);

    vec3 base_color = degamma(texture(albedo_tex, TexCoords).rgb);
    vec3 normal = normalize(texture(normal_tex, TexCoords).rgb);
    vec3 position = texture(position_tex, TexCoords).rgb;
    vec3 world_sampler_pos = vec3((position.x + 0.5) / 256.0, (position.z + 0.5) / 256.0, (position.y + 0.5) / 128.0);

    // Material definitions
    vec3 material_lookup = texture(ao_tex, TexCoords).rgb;
    float ambient_occlusion = material_lookup.r;
    float metallic = material_lookup.g;
    float roughness = material_lookup.b;
    vec3 specular_color = mix(vec3(0.04), base_color, metallic);

    // Retrieve information from the world texture
    vec3 outdoor_x_y = texture(info_tex, world_sampler_pos).rgb;
    vec3 light_position = texture(light_pos_tex, world_sampler_pos).rgb * 256.0;
    light_position.y += 0.48;
    vec3 light_color = degamma(texture(light_col_tex, world_sampler_pos).rgb);

    // Output components
    vec3 ambient_ref = base_color * ambient_occlusion;
    vec3 diffuse_ref = vec3(0.0);
    vec3 specular_ref = vec3(0.0);

    // Calculated variables
    vec3 Lsun = normalize(sun_position - position);     // Lx - light position minus world position
    vec3 Llight = normalize(light_position - position);
    vec3 V = normalize(-position); // V - negative position
    vec3 Hsun = normalize(Lsun + V);
    vec3 Hlight = normalize(Llight + V);
    vec3 N = normal;

    float NdLsun = max(0.0, dot(N, Lsun));
    float NdLlight = max(0.0, dot(N, Llight));
    float NdV = max(0.001, dot(N, V));
    float NdHsun = max(0.001, dot(N, Hsun));
    float NdHlight = max(0.001, dot(N, Hlight));
    float HsundV = max(0.001, dot(Hsun, V));
    float HlightdV = max(0.001, dot(Hlight, V));
    float LsundV = max(0.001, dot(Lsun, V));
    float LlightdV = max(0.001, dot(Lsun, V));

    // Sunlight
    vec3 sun_diffuse_color = base_color;
    diffuse_ref += outdoor_x_y.r > 0.0 ? lambert_diffuse(NdLsun, base_color, sun_color) : vec3(0.0);
    vec3 specSunfresnel = fresnel_factor(specular_color, HsundV);
    specular_ref += cooktorrance_specular(NdLsun, NdV, NdHsun, specSunfresnel, roughness) * NdLsun;

    // Game lights
    vec3 light_diffuse_color = base_color;
    float light_distance = distance(light_position, position);
    float attenuation = 1.0/light_distance;
    diffuse_ref += lambert_diffuse(NdLlight, base_color, light_color) * attenuation;
    vec3 specLightfresnel = fresnel_factor(specular_color, HlightdV);
    specular_ref += cooktorrance_specular(NdLlight, NdV, NdHlight, specLightfresnel, roughness) * NdLlight;

    // Limit the total light delivery
    //vec3 diffuse_amount = (vec3(1.0) - (specLightfresnel + specSunfresnel));

    // Final color
    //vec3 final_color = ambient + diffuse;
    vec3 final_color = (ambient_ref * 0.3) + (diffuse_ref * 0.7) + specular_ref;
    FragColor = final_color;

    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0) BrightColor = FragColor.rgb;
}
