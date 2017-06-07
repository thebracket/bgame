#version 120

uniform sampler2D my_color_texture;
uniform sampler2D shadow_map;
varying vec3 base_normal;
varying vec3 tint;
varying vec3 world_pos;
varying vec4 fragment_light_space_position;

float shadow_calculation(vec4 frag_pos_light_space) {
    vec3 proj_coords = frag_pos_light_space.xyz / frag_pos_light_space.w;
    proj_coords = proj_coords * 0.5 + 0.5; // Convert to depth map space
    float closest_depth = texture2D(shadow_map, proj_coords.xy).r;
    float current_depth = proj_coords.z;
    return current_depth - 0.005 < closest_depth ? 1.0 : 0.0;
    //return closest_depth;
}

void main() {
    gl_FragData[0] = vec4(world_pos.x / 255, world_pos.y / 255, world_pos.z / 255, 1.0);
    vec4 color = vec4(tint.r, tint.g, tint.b + base_normal.z, 1.0);
    gl_FragData[1] = texture2D(my_color_texture, gl_TexCoord[0].st) * color;
    gl_FragData[2] = vec4(base_normal.xyz, 1.0);
    float shadow = shadow_calculation(fragment_light_space_position);
    gl_FragData[3] = vec4(shadow, shadow, shadow, 1.0);
}
