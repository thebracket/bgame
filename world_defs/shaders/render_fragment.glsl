#version 120

uniform sampler2D albedo_tex;
uniform sampler2D position_tex;
uniform sampler2D normal_tex;
uniform sampler2D shadow_map;

uniform vec3 cameraPosition;
uniform vec3 ambient_color;
uniform vec3 sun_moon_color;
uniform vec3 sun_moon_position;

vec3 diffuse_light(vec3 light_pos, vec3 surface_pos, vec3 surface_normal, vec3 color) {
    vec3 light_dir = normalize(light_pos - surface_pos);
    color.xyz *= dot(light_dir, surface_normal);
    return color;
}

void main() {
    vec4 base_color = texture2D( albedo_tex, gl_TexCoord[0].xy );
    vec4 position = texture2D( position_tex, gl_TexCoord[0].xy );
    vec4 normal = texture2D( normal_tex, gl_TexCoord[0].xy );
    vec4 shadow = texture2D( shadow_map, gl_TexCoord[0].xy );

    normal = normalize(normal);

    if (shadow.r < 1.0) {
        base_color.xyz *= ambient_color;
    } else {
        base_color.xyz *= diffuse_light(sun_moon_position, position.xyz, normal.xyz, sun_moon_color) + ambient_color;
    }
    //base_color.xyz *= ambient_color;

    gl_FragColor = base_color;
}