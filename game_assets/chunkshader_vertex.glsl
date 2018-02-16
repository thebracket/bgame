#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 texture_info;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBiTangent;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;

out vec3 tex_pos;
out vec3 world_pos;
out mat3 TBN;
out vec3 base_normal;

void main()
{
    gl_Position = projection_matrix * (view_matrix * vec4(aPos, 1.0));
    tex_pos = texture_info;
    world_pos = aPos;

    vec3 T = normalize(aTangent);
    vec3 B = normalize(aBiTangent);
    vec3 N = normalize(aNormal);
    TBN = mat3(T, B, N);

    base_normal = normalize(aNormal);
}
