#version 120

uniform sampler2D my_color_texture;
uniform sampler2D my_normal_texture;
varying vec3 tint;
varying vec3 world_pos;
varying mat3 TBN;
varying vec4 interpolated_position;
varying vec3 flag_out;
varying vec3 light_pos;
varying vec3 light_col;

void main() {
    vec3 calc_normal = texture2D(my_normal_texture, gl_TexCoord[0].st).rgb;
    calc_normal = normalize(calc_normal * 2.0 - 1.0);
    calc_normal = normalize(TBN * calc_normal);

    gl_FragData[0] = vec4(world_pos.x / 255, world_pos.y / 255, world_pos.z / 255, 1.0);
    vec4 color = vec4(tint.r, tint.g, tint.b, 1.0);
    gl_FragData[1] = texture2D(my_color_texture, gl_TexCoord[0].st) * color;
    gl_FragData[2] = vec4(calc_normal.xyz, 1.0);
    gl_FragData[3] = interpolated_position / 255.0;
    gl_FragData[5] = vec4(light_pos, 1.0);
    gl_FragData[4] = vec4(light_col, 1.0);
    gl_FragData[6] = vec4(flag_out.r, flag_out.g, 0.0, 1.0);
}
