#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 3) in vec3 instancePos;

uniform mat4 combined_matrix;
uniform float texSize;

out vec3 world_pos;

void main()
{
    vec3 shrunk_pos = (aPos - texSize/2.0) / texSize;
    world_pos = shrunk_pos + instancePos;
    gl_Position = combined_matrix * vec4(world_pos, 1.0);
}
