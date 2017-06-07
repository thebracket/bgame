#version 120

uniform sampler2D my_color_texture;
varying vec3 base_normal;
varying vec3 tint;
varying vec3 world_pos;

void main() {
    gl_FragData[0] = vec4(world_pos.x / 255, world_pos.y / 255, world_pos.z / 255, 1.0);
    vec4 color = vec4(tint.r, tint.g, tint.b + base_normal.z, 1.0);
    gl_FragData[1] = texture2D(my_color_texture, gl_TexCoord[0].st) * color;
    gl_FragData[2] = vec4(base_normal.xyz, 1.0);
}
