#version 120

uniform sampler2D intermediate_tex;
uniform float gamma;

varying vec4 tex_coord;

void main() {
    vec3 base_color = pow(texture2D( intermediate_tex, tex_coord.xy ).rgb, vec3(1.0/gamma));
    gl_FragData[0] = vec4(base_color, 1.0f);
}