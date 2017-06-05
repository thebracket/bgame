#version 120

uniform sampler2D my_color_texture;
varying vec3 base_normal;
varying vec3 tint;

void main() {
    vec4 color = vec4(tint.r, tint.g, tint.b + base_normal.z, 1.0);
    gl_FragColor = texture2D(my_color_texture, gl_TexCoord[0].st) * color;
}
