#version 430 core
layout (location = 0) out vec3 FragColor;

in vec2 TexCoords;

uniform vec3 light_position;
uniform float far_plane;
uniform sampler2D world_position;
uniform samplerCube depthMap;
uniform sampler2D albedo_tex;
uniform sampler2D normal_tex;
uniform sampler2D ao_tex;

uniform vec3 camera_position;
uniform vec3 light_color;

#define PI 3.1415926

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
    const float diskRadius = 0.2;
    const float bias = 0.15;
    const int samples = 20;
    //const int samples = 1;
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

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 calculateLightOutput(vec3 albedo, vec3 N, vec3 V, vec3 F0, vec3 L, vec3 radiance, float roughness, float metallic) {
    vec3 H = normalize(V + L);
    float NDF = DistributionGGX(N, H, roughness);   
    float G   = GeometrySmith(N, V, L, roughness);      
    vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);
        
    vec3 nominator    = NDF * G * F; 
    float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; // 0.001 to prevent divide by zero.
    vec3 specular = nominator / denominator;
    
    // kS is equal to Fresnel
    vec3 kS = F;
    // for energy conservation, the diffuse and specular light can't
    // be above 1.0 (unless the surface emits light); to preserve this
    // relationship the diffuse component (kD) should equal 1.0 - kS.
    vec3 kD = vec3(1.0) - kS;
    // multiply kD by the inverse metalness such that only non-metals 
    // have diffuse lighting, or a linear blend if partly metal (pure metals
    // have no diffuse light).
    kD *= 1.0 - metallic;	  

    // scale light by NdotL
    float NdotL = max(abs(dot(N, L)), 0.0);

    return (kD * albedo / PI + specular) * radiance * NdotL;
}

vec3 gameLight(vec3 albedo, vec3 N, vec3 V, vec3 F0, float roughness, float metallic, vec3 light_position, vec3 position, vec3 light_color) {
    vec3 L = normalize(light_position.xyz - position);    
    float distance = distance(light_position.xyz, position);
    float attenuation = far_plane > 64.0 ? 1.0 : (1.0 / (distance));
    vec3 radiance = light_color * attenuation;

    return calculateLightOutput(albedo, N, V, F0, L, radiance, roughness, metallic);
}

void main()
{
    vec3 position = texture(world_position, TexCoords).rgb;
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

    // Lighting
    vec3 base_color = texture(albedo_tex, TexCoords).rgb;
    vec3 normal = normalize(texture(normal_tex, TexCoords).rgb);

    // Material definitions
    vec3 material_lookup = texture(ao_tex, TexCoords).rgb;
    float ambient_occlusion = material_lookup.r;
    float metallic = material_lookup.g;
    float roughness = material_lookup.b;
    vec3 specular_color = mix(vec3(0.04), base_color, metallic);

    // Use https://learnopengl.com/code_viewer_gh.php?code=src/6.pbr/1.2.lighting_textured/1.2.pbr.fs
    vec3 albedo = base_color;
    vec3 N = normal;
    vec3 V = normalize(camera_position - position);
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    vec3 Lo = vec3(0.0); // Light Output
    Lo += gameLight(albedo, N, V, F0, roughness, metallic, light_position.xyz + vec3(0.0, 0.5, 0.0), position.xyz, light_color.rgb);

    FragColor = Lo * shadowFactor;
}
