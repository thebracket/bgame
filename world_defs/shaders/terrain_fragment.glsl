#version 120

uniform sampler2D my_color_texture;
uniform sampler2D my_normal_texture;
uniform sampler2D my_wang_texture;
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
    if (flag_out.b == 0 || flag_out.b == 15) {
        gl_FragData[1] = texture2D(my_color_texture, gl_TexCoord[0].st) * color;
    } else {
        // Modify alpha by wang. Need to calculate wang co-ordinates here.
        int wang_tile = int(flag_out.b - 1);
        float wang_x = mod(wang_tile, 4);
        float wang_y = floor(wang_tile / 4);
        float wangx = wang_x / 4.0;
        float wangy = wang_y / 4.0;
        vec2 wang_vec = vec2(wangx, wangy);

        vec3 wang = texture2D(my_wang_texture, wang_vec).rgb;
        vec4 outcol = texture2D(my_color_texture, gl_TexCoord[0].st) * color;
        outcol.a = wang.r;
        gl_FragData[1] = outcol;
    }
    gl_FragData[2] = vec4(calc_normal.xyz, 1.0);
    gl_FragData[3] = interpolated_position / 255.0;
    gl_FragData[5] = vec4(light_pos, 1.0);
    gl_FragData[4] = vec4(light_col, 1.0);
    gl_FragData[6] = vec4(flag_out.rgb/255.0, 1.0);
}
