#version 330 core

uniform sampler2D ascii_tex;
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gAlbedo;

in vec3 world_pos;
in vec2 tex_pos;
in vec3 fg;
in vec3 bg;

void main() {
    vec4 pixel_color = texture(ascii_tex, tex_pos);
    vec3 out_color = pixel_color.r > 0.5 ? fg * pixel_color.rgb : bg;
    gAlbedo = out_color;
    gPosition = world_pos;
    //gAlbedo = vec3(1.0, 1.0, 1.0);
}