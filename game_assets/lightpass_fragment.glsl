#version 330 core
out vec4 FragColor;

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

vec3 lambert_diffuse(vec3 L, vec3 N, vec3 base_color, vec3 light_color) {
    float diff = max(dot(N, L), 0.0);
    return diff * base_color * light_color;
}

void main()
{
    // TODO: Make this a uniform
    vec3 sun_position = vec3(128.0, 256.0, 128.0);
    vec3 sun_color = vec3(1.0);

    vec3 base_color = texture(albedo_tex, TexCoords).rgb;
    vec3 normal = normalize(texture(normal_tex, TexCoords).rgb);
    vec3 position = texture(position_tex, TexCoords).rgb;
    vec3 world_sampler_pos = vec3((position.x + 0.5) / 256.0, (position.z + 0.5) / 256.0, (position.y + 0.5) / 128.0);

    // Retrieve information from the world texture
    vec3 outdoor_x_y = texture(info_tex, world_sampler_pos).rgb;
    vec3 light_position = texture(light_pos_tex, world_sampler_pos).rgb * 256.0;
    vec3 light_color = texture(light_col_tex, world_sampler_pos).rgb;

    // Output components
    vec3 ambient_ref = base_color * texture(ao_tex, TexCoords).r;
    vec3 diffuse_ref = vec3(0.0);
    vec3 specular_ref = vec3(0.0);

    // Calculated variables
    vec3 Lsun = normalize(sun_position - position);     // Lx - light position minus world position
    vec3 Llight = normalize(light_position - position);

    // Sunlight
    diffuse_ref += outdoor_x_y.r > 0.0 ? lambert_diffuse(Lsun, normal, base_color, sun_color) : vec3(0.0);

    // Game lights
    float light_distance = distance(light_position, position);
    float attenuation = 1.0/light_distance;
    diffuse_ref += lambert_diffuse(Llight, normal, base_color, light_color);

    /*
    // Calculate initial ambient
    float ambient_strength = 0.3;
    float ambient_occluded = texture(ao_tex, TexCoords).r;
    vec3 ambient = base_color * ambient_strength * ambient_occluded;

    // Lambert lighting for diffuse
    float diffuse_strength = 0.5;
    vec3 lightDir = normalize(light_position - position);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = base_color * diff * light_color;

    vec3 eyeDir = normalize(camera_position - position);*/

    // Final color
    //vec3 final_color = ambient + diffuse;
    vec3 final_color = (ambient_ref * 0.3) + (diffuse_ref * 0.7) + specular_ref;
    FragColor = vec4(final_color, 1.0);
}
