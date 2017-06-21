#version 120

uniform sampler2D albedo_tex;
uniform sampler2D position_tex;
uniform sampler2D normal_tex;

uniform vec3 cameraPosition;
uniform vec3 ambient_color;

vec3 light_dir;

vec3 diffuse_light(vec3 light_pos, vec3 surface_pos, vec3 surface_normal, vec3 color) {
    vec3 result = color;
    result.xyz *= dot(light_dir, surface_normal);
    return result;
}

vec3 specular_light(vec3 light_pos, vec3 surface_pos, vec3 surface_normal, vec3 color) {
    float specular_strength = 0.5;
    float shininess = 8;

    vec3 view_dir = normalize(cameraPosition - surface_pos);
    vec3 reflect_dir = reflect(-light_dir, surface_normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), shininess);
    vec3 result = specular_strength * spec * color;
    return result;
}

void main() {
    vec4 base_color = texture2D( albedo_tex, gl_TexCoord[0].xy );
    vec4 position = texture2D( position_tex, gl_TexCoord[0].xy ) * 255.0;
    vec4 normal = texture2D( normal_tex, gl_TexCoord[0].xy );

    normal = normalize(normal);
    //light_dir = normalize(sun_moon_position - position.xyz);
    base_color.xyz *= ambient_color;

    gl_FragColor = base_color;
}