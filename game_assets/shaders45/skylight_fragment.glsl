#version 430 core
layout (location = 0) out vec3 FragColor;

in vec2 TexCoords;

uniform vec3 light_position;
uniform sampler2D world_position;
uniform sampler2D albedo_tex;
uniform sampler2D normal_tex;
uniform sampler2D ao_tex;

uniform vec3 camera_position;
uniform vec3 light_color;

layout (std430, binding=4) readonly buffer terrain_flags
{ 
    uint flags[];
} terrainFlags;

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

vec3 degamma(vec3 col) {
    return pow(col, vec3(2.2));
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
    vec3 L = normalize(light_position.xyz);    
    vec3 radiance = light_color;

    return calculateLightOutput(albedo, N, V, F0, L, radiance, roughness, metallic);
}

uint mapidx(vec3 position) {
    int xi = int(ceil(position.x));
    int yi = int(ceil(position.y));
    int zi = int(ceil(position.z));

    return (zi * 65536) + (yi * 256) + xi;
}

void main()
{
    vec4 position_tmp = texture(world_position, TexCoords).rgba;
    vec3 position = vec3(position_tmp.rgb);

    // Discard if the sun isn't up
    vec3 fragToLight = normalize( light_position );
    float is_lit = max(dot(fragToLight, vec3(0.0, 1.0, 0.0)), 0.0);
    if (is_lit < 0.1) discard;

    // Discard if we're not above ground
    uint idx = mapidx(position_tmp.rba);
    uint above_ground = terrainFlags.flags[idx] & 2048;
    if (above_ground == 0) discard;    

    // Lighting
    vec3 base_color = texture(albedo_tex, TexCoords).rgb;
    vec3 normal = normalize(texture(normal_tex, TexCoords).rgb);

    uint visible = terrainFlags.flags[idx] & 512;
    if (visible == 0) base_color = vec3(dot(base_color.rgb, vec3(0.299, 0.587, 0.114)));

    // Material definitions
    vec3 material_lookup = texture(ao_tex, TexCoords).rgb;
    float ambient_occlusion = material_lookup.r;
    float metallic = material_lookup.g;
    float roughness = material_lookup.b;
    vec3 specular_color = mix(vec3(0.04), base_color, metallic);

    // Use https://learnopengl.com/code_viewer_gh.php?code=src/6.pbr/1.2.lighting_textured/1.2.pbr.fs
    vec3 albedo = base_color;
    vec3 N = normal;
    vec3 V = normalize(camera_position - vec3(128, 64, 128));
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    vec3 Lo = vec3(0.0); // Light Output
    Lo += gameLight(albedo, N, V, F0, roughness, metallic, light_position.xyz, position.xyz, light_color.rgb);
    float y_difference = 1.0 - ((camera_position.y - position.y) * 0.1);
    FragColor = is_lit * Lo * y_difference;
}
