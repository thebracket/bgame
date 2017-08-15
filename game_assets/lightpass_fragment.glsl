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

void main()
{
    vec3 base_color = texture(albedo_tex, TexCoords).rgb;
    vec3 normal = normalize(texture(normal_tex, TexCoords).rgb);
    vec3 position = texture(position_tex, TexCoords).rgb * 256.0;
    vec3 world_sampler_pos = vec3((position.x + 0.5) / 256.0, (position.z + 0.5) / 256.0, (position.y + 0.5) / 128.0);

    // Retrieve information from the world texture
    vec3 outdoor_x_y = texture(info_tex, world_sampler_pos).rgb;
    vec3 light_position = texture(light_pos_tex, world_sampler_pos).rgb;
    vec3 light_color = texture(light_col_tex, world_sampler_pos).rgb;

    vec3 diffuse_ref = vec3(0.0);

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
    FragColor = vec4(light_position, 1.0);
}
