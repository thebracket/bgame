#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D albedo_tex;
uniform sampler2D normal_tex;
uniform sampler2D position_tex;
uniform sampler2D light_position_tex;
uniform sampler2D light_color_tex;

uniform vec3 camera_position;

void main()
{
    vec3 base_color = texture(albedo_tex, TexCoords).rgb;
    vec3 normal = normalize(texture(normal_tex, TexCoords).rgb);
    vec3 position = texture(position_tex, TexCoords).rgb * 256.0;
    vec3 light_position = texture(light_position_tex, TexCoords).rgb * 256.0;
    vec3 light_color = texture(light_color_tex, TexCoords).rgb;

    // Calculate initial ambient
    float ambient_strength = 0.3;
    vec3 ambient = base_color * ambient_strength;

    // Lambert lighting for diffuse
    float diffuse_strength = 0.5;
    vec3 lightDir = normalize(light_position - position);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = base_color * diff * light_color;

    // Final color
    vec3 final_color = ambient + diffuse;
    FragColor = vec4(final_color, 1.0);
}
