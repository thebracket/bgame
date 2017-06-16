#version 120

uniform sampler2D my_color_texture;
uniform sampler2D shadow_map;
varying vec3 tint;
varying vec3 world_pos;
varying mat3 TBN;
varying vec4 interpolated_position;

/*float shadow_calculation(vec4 frag_pos_light_space) {
    vec3 light_dir = normalize(frag_pos_light_space.xyz - world_pos);

    vec3 proj_coords = frag_pos_light_space.xyz / frag_pos_light_space.w;
    proj_coords = proj_coords * 0.5 + 0.5; // Convert to depth map space
    float closest_depth = texture2D(shadow_map, proj_coords.xy).r;
    float current_depth = proj_coords.z;
    float bias = 0.01;
    return current_depth - bias > closest_depth ? 0.0 : 1.0;
    //return closest_depth;
}*/

void main() {
    vec3 calc_normal = texture2D(my_color_texture, gl_TexCoord[1].st).rgb;
    calc_normal = normalize(calc_normal * 2.0 - 1.0);
    calc_normal = normalize(TBN * calc_normal);

    gl_FragData[0] = vec4(world_pos.x / 255, world_pos.y / 255, world_pos.z / 255, 1.0);
    vec4 color = vec4(tint.r, tint.g, tint.b, 1.0);
    gl_FragData[1] = texture2D(my_color_texture, gl_TexCoord[0].st) * color;
    gl_FragData[2] = vec4(calc_normal.xyz, 1.0);
    gl_FragData[3] = interpolated_position / 255.0;
}
