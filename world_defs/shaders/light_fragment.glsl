#version 120

uniform sampler2D shadow_map;
uniform vec3 light_color;
uniform vec3 light_position;
uniform float range;
varying vec3 world_pos;
varying vec4 fragment_light_space_position;

void main() {
    vec3 distance_vec = light_position - world_pos;
    if (abs(length(distance_vec)) < range) {
        vec3 light_dir = normalize(fragment_light_space_position.xyz - world_pos);
        vec3 proj_coords = fragment_light_space_position.xyz / fragment_light_space_position.w;
        proj_coords = proj_coords * 0.5 + 0.5; // Convert to depth map space
        float closest_depth = texture2D(shadow_map, proj_coords.xy).r;
        float current_depth = proj_coords.z;
        float bias = 0.01;
        if (current_depth - bias < closest_depth) {
            gl_FragData[1] = vec4(light_color, 1.0);
            gl_FragData[0] = vec4(light_position / 255.0, 1.0);
        }
    }
}
