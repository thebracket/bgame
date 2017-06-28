#version 120

uniform sampler2D my_color_texture;
varying vec3 tint;
varying vec3 world_pos;
varying vec4 interpolated_position;
varying vec3 flag_out;
varying vec3 light_pos;
varying vec3 light_col;
varying vec3 normal_out;

void main() {
    vec3 calc_normal = normalize(normal_out);

    gl_FragData[0] = vec4(world_pos.x / 255, world_pos.y / 255, world_pos.z / 255, 1.0);
    vec4 color = vec4(tint.r, tint.g, tint.b, 1.0);
    gl_FragData[1] = texture2D(my_color_texture, gl_TexCoord[0].st) * color;
    gl_FragData[2] = vec4(calc_normal.xyz, 1.0);
    gl_FragData[3] = interpolated_position / 255.0;
    gl_FragData[5] = vec4(light_pos, 1.0);
    gl_FragData[4] = vec4(light_col, 1.0);
    gl_FragData[6] = vec4(flag_out.rgb/255.0, 1.0);
}
