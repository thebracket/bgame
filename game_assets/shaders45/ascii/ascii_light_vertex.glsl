#version 330 core

// Incoming layout
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 wPos;
layout (location = 2) in vec2 aTex;
layout (location = 3) in vec3 aForeground;
layout (location = 4) in vec3 aBackground;

// Uniforms
uniform mat4 projection_matrix;
uniform mat4 view_matrix;

// Send to vertex shader
out VS_OUT {
    out vec3 world_pos;
    out vec2 tex_pos;
    out vec3 fg;
    out vec3 bg;
} vs_out;

void main()
{
    gl_Position = projection_matrix * ( view_matrix * vec4(aPos.x, wPos.z, aPos.y, 1.0));
    vs_out.world_pos = wPos;
    vs_out.tex_pos = aTex;
    vs_out.fg = aForeground;
    vs_out.bg = aBackground;
}