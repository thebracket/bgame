#version 120

uniform sampler2D albedo_tex;
uniform sampler2D position_tex;
uniform sampler2D normal_tex;
uniform sampler2D light_pos_tex;
uniform sampler2D light_col_tex;
uniform sampler2D flag_tex;

uniform vec3 cameraPosition;
uniform vec3 ambient_color;
uniform vec3 sun_moon_position;
uniform vec3 sun_moon_color;

varying vec4 tex_coord;

vec3 diffuse_light(vec3 surface_normal, vec3 color, vec3 light_dir) {
    float diffuse_strength = 0.5;
    vec3 result = color;
    result.xyz *= dot(light_dir, surface_normal) * diffuse_strength;
    return result;
}

vec3 specular_light(vec3 surface_pos, vec3 surface_normal, vec3 color, vec3 light_dir, float shininess) {
    float specular_strength = 0.7;
    shininess = 2;

    /*vec3 view_dir = normalize(cameraPosition - surface_pos);
    vec3 reflect_dir = reflect(-light_dir, surface_normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), shininess);
    vec3 result = specular_strength * spec * color;
    return result;*/
    vec3 incidence_vector = -light_dir;
    vec3 reflection_vector = reflect(incidence_vector, surface_normal);
    vec3 surface_to_camera = normalize(cameraPosition - surface_pos);
    float cos_angle = max(0.0, dot(surface_to_camera, reflection_vector));
    float specular_coefficient = pow(cos_angle, shininess);
    vec3 specular_component = specular_coefficient * color * specular_strength;
    return specular_component;
}

void main() {
    vec4 base_color = texture2D( albedo_tex, tex_coord.xy );
    vec4 position = texture2D( position_tex, tex_coord.xy ) * 255.0;
    vec4 normal = texture2D( normal_tex, tex_coord.xy );
    normal = normalize(normal);

    vec4 light_position = texture2D( light_pos_tex, tex_coord.xy ) * 255.0;
    vec3 light_color = texture2D( light_col_tex, tex_coord.xy ).rgb;
    vec3 flags = texture2D( flag_tex, tex_coord.xy ).rgb * 255.0;

    // Calculate the ambient component
    vec3 ambient = flags.r > 0.0 ? ambient_color : vec3(0.3, 0.3, 0.3);
    base_color.xyz *= ambient;

    // Apply light from the sun
    vec3 sun_dir = normalize(sun_moon_position.xyz - position.xyz);
    vec3 sun_diffuse = diffuse_light(normal.xyz, sun_moon_color, sun_dir);
    vec3 sun_specular = specular_light(position.xyz, normal.xyz, sun_moon_color.rgb, sun_dir, flags.g);
    float sun_diffuse_factor = flags.r > 0.0 ? 1.0 : 0.0;
    float sun_specular_factor = flags.r > 0.0 && flags.b < 1.0 ? 1.0 : 0.0;
    base_color.xyz += sun_diffuse * sun_diffuse_factor;
    base_color.xyz += sun_specular * sun_specular_factor;

    // Apply light from in-game light sources
    vec3 light_dir = normalize(light_position.xyz - position.xyz);
    base_color.xyz += diffuse_light(normal.xyz, light_color, light_dir);
    float light_component = light_position.x > 0.0 && flags.b < 1.0f ? 1.0 : 0.0;
    base_color.xyz += specular_light(position.xyz, normal.xyz, light_color, light_dir, flags.g) * light_component;

    gl_FragColor = base_color;
}