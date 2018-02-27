#version 430 core

uniform sampler2D ascii_tex;
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gAlbedo;

// Receive from vertex shader
in VS_OUT {
    vec3 world_pos;
    vec2 tex_pos;
    vec3 fg;
    vec3 bg;
} fs_in;

void main() {
    vec4 pixel_color = texture(ascii_tex, fs_in.tex_pos);
    vec3 out_color = pixel_color.r > 0.5 ? fs_in.fg * pixel_color.rgb : fs_in.bg;
    gAlbedo = out_color;
    gPosition = fs_in.world_pos;
    //gAlbedo = vec3(1.0, 1.0, 1.0);
}