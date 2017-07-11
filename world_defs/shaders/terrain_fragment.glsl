#version 120

uniform sampler2D my_color_texture;
uniform vec3 camera_position;

varying vec3 tint;
varying vec3 world_pos;
varying mat3 TBN;
varying vec4 interpolated_position;
varying vec3 flag_out;
varying vec3 light_pos;
varying vec3 light_col;
varying vec4 normal_tex_position;
varying vec4 specular_tex_position;
varying vec4 displacement_tex_position;

vec2 parallax_mapping(vec2 tex_coords, vec3 view_dir) {
    const float height_scale = 0.1f;
    float height = texture2D(my_color_texture, displacement_tex_position.st).r;
    vec2 p = view_dir.xy / view_dir.z * (height * height_scale);
    return tex_coords - p;
}

void main() {
    vec3 tangent_view_pos = TBN * camera_position;
    vec3 tangent_frag_pos = TBN * interpolated_position.xyz;
    vec3 view_dir = normalize(tangent_view_pos - tangent_frag_pos);
    vec2 tex_coords_t = parallax_mapping(gl_TexCoord[0].st, view_dir);
    vec2 tex_coords_n = parallax_mapping(normal_tex_position.st, view_dir);
    vec2 tex_coords_s = parallax_mapping(specular_tex_position.st, view_dir);

    vec3 calc_normal = texture2D(my_color_texture, tex_coords_n).rgb;
    calc_normal = normalize(calc_normal * 2.0 - 1.0);
    calc_normal = normalize(TBN * calc_normal);

    gl_FragData[0] = vec4(world_pos.x / 255, world_pos.y / 255, world_pos.z / 255, 1.0);
    vec4 color = vec4(tint.r, tint.g, tint.b, 1.0);
    gl_FragData[1] = texture2D(my_color_texture, tex_coords_t) * color;
    gl_FragData[2] = vec4(calc_normal.xyz, 1.0);
    gl_FragData[3] = interpolated_position / 255.0;
    gl_FragData[5] = vec4(light_pos, 1.0);
    gl_FragData[4] = vec4(light_col, 1.0);
    gl_FragData[6] = vec4(flag_out.rgb/255.0, 1.0);
    gl_FragData[7] = texture2D(my_color_texture, tex_coords_s);
}
