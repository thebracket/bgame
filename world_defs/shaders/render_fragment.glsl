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

vec3 diffuse_light(vec3 surface_normal, vec3 color, vec3 light_dir) {
    vec3 result = color;
    result.xyz *= dot(light_dir, surface_normal);
    return result;
}

vec3 specular_light(vec3 surface_pos, vec3 surface_normal, vec3 color, vec3 light_dir, float shininess) {
    float specular_strength = 0.5;

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

    vec4 light_position = texture2D( light_pos_tex, gl_TexCoord[0].xy ) * 255.0;
    vec3 light_color = texture2D( light_col_tex, gl_TexCoord[0].xy ).rgb;
    vec3 flags = texture2D( flag_tex, gl_TexCoord[0].xy ).rgb;

    // Apply ambient color
    if (flags.r > 0.0) {
        // We are outdoors, use ambient light from the sun/moon
        base_color.xyz *= ambient_color;

        // Add sun/moon light
        vec3 light_dir = normalize(sun_moon_position.xyz - position.xyz);
        base_color.xyz += diffuse_light(normal.xyz, sun_moon_color, light_dir);
        base_color.xyz += specular_light(position.xyz, normal.xyz, sun_moon_color.rgb, light_dir, flags.g);
    } else {
        base_color.xyz *= vec3(0.2, 0.2, 0.2);
    }

    // If the tile is lit, we need to apply the lighting
    if (light_position.x > 0.0) {
        vec3 light_dir = normalize(light_position.xyz - position.xyz);
        base_color.xyz += diffuse_light(normal.xyz, light_color, light_dir);
        base_color.xyz += specular_light(position.xyz, normal.xyz, light_color.rgb, light_dir, flags.g);
    }


    gl_FragColor = base_color;
}