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
uniform sampler2D sun_depth_tex;
uniform sampler2D moon_depth_tex;

uniform vec3 camera_position;
uniform vec3 sun_direction;
uniform vec3 sun_color;
uniform vec3 moon_direction;
uniform vec3 moon_color;
uniform mat4 sun_projection;
uniform mat4 sun_modelview;
uniform mat4 moon_projection;
uniform mat4 moon_modelview;

#define PI 3.1415926

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
    float NdotL = max(dot(N, L), 0.0);

    return (kD * albedo / PI + specular) * radiance * NdotL;
}

vec3 gameLight(vec3 albedo, vec3 N, vec3 V, vec3 F0, float roughness, float metallic, vec3 light_position, vec3 position, vec3 light_color) {
    vec3 L = normalize(light_position.xyz - position);    
    float distance = length(light_position.xyz - position);
    float attenuation = (1.0 / (distance)) * 1.5;
    vec3 radiance = light_color * attenuation;

    return calculateLightOutput(albedo, N, V, F0, L, radiance, roughness, metallic);
}

vec3 celestialLight(vec3 albedo, vec3 N, vec3 V, vec3 F0, float roughness, float metallic, vec3 light_direction, vec3 light_color) {
    vec3 L = normalize(light_direction);    

    return calculateLightOutput(albedo, N, V, F0, L, light_color, roughness, metallic);
}

float celestialShadow(mat4 proj, mat4 modelview, vec3 position, vec3 N, vec3 light_direction, sampler2D depth_tex) 
{
    vec4 fragPosLightSpace = proj * (modelview * vec4(position, 1.0));
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5; 
    float closestDepth = texture(depth_tex, projCoords.xy).r;     
    float currentDepth = projCoords.z;
    //float bias = 0.005;
    float bias = max(0.05 * (1.0 - dot(N, normalize(light_direction))), 0.005); 
    float shadow = currentDepth - bias > closestDepth  ? 0.0 : 1.0; 
    return shadow;
}

void main()
{
    vec3 base_color = degamma(texture(albedo_tex, TexCoords).rgb);
    vec3 normal = normalize(texture(normal_tex, TexCoords).rgb);
    vec3 position = texture(position_tex, TexCoords).rgb;
    vec3 world_sampler_pos = vec3((position.x + 0.5) / 256.0, (position.z + 0.5) / 256.0, (position.y + 0.6) / 128.0);

    // Material definitions
    vec3 material_lookup = texture(ao_tex, TexCoords).rgb;
    float ambient_occlusion = material_lookup.r;
    float metallic = material_lookup.g;
    float roughness = material_lookup.b;
    vec3 specular_color = mix(vec3(0.04), base_color, metallic);

    // Retrieve information from the world texture
    vec3 outdoor_x_y = texture(info_tex, world_sampler_pos).rgb;
    vec3 light_position = texture(light_pos_tex, world_sampler_pos).rgb * 256.0;
    light_position.y += 0.5;
    vec3 light_color = degamma(texture(light_col_tex, world_sampler_pos).rgb);

    // Use https://learnopengl.com/code_viewer_gh.php?code=src/6.pbr/1.2.lighting_textured/1.2.pbr.fs
    vec3 albedo = degamma(texture(albedo_tex, TexCoords).rgb);
    vec3 N = normal;
    vec3 V = normalize(camera_position - position);
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    vec3 Lo = vec3(0.0); // Light Output

    // For the game-defined light
    vec3 gamelighttemp = gameLight(albedo, N, V, F0, roughness, metallic, light_position.xyz, position.xyz, light_color.rgb);

    // Sun/moon
    vec3 suntemp = celestialLight(albedo, N, V, F0, roughness, metallic, sun_direction, sun_color);
    vec3 moontemp = celestialLight(albedo, N, V, F0, roughness, metallic, moon_direction, moon_color);

    Lo += ( suntemp * celestialShadow(sun_projection, sun_modelview, position, N, sun_direction, sun_depth_tex) );
    Lo += ( moontemp * celestialShadow(moon_projection, moon_modelview, position, N, moon_direction, moon_depth_tex) );

    // Final color
    vec3 ambient = albedo * ambient_occlusion;
    FragColor = ambient + Lo;

    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0) BrightColor = FragColor.rgb;
}
