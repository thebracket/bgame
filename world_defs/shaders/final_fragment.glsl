#version 120

uniform sampler2D intermediate_tex;
uniform float exposure;

varying vec4 tex_coord;

void main() {
    vec3 hdr_color = texture2D(intermediate_tex, tex_coord.st).rgb;

    // Simple exposure-based tone mapping
    vec3 mapped = vec3(1.0) - exp( -hdr_color * exposure );

    // Gamma correction
    //mapped = pow(mapped, vec3(1.0/gamma));

    // Add some scan-line noise
    //float scan_mod = mod(gl_FragCoord.y, 4.0);
    //float scan_effect = 1.0 - (scan_mod/20.0f);
    //mapped.rgb *= scan_effect;

    gl_FragData[0] = vec4(mapped, 1.0);
}