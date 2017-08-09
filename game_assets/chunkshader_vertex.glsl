#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 texture_info;
layout (location = 2) in vec3 aNormal;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;

out vec3 tex_pos;
out vec3 world_pos;
out mat3 TBN;

void main()
{
    gl_Position = projection_matrix * (view_matrix * vec4(aPos, 1.0));
    tex_pos = texture_info;
    world_pos = aPos;

    vec3 tangent;
        vec3 bitangent;

        vec3 c1 = cross(aNormal, vec3(0.0, 0.0, 1.0));
        vec3 c2 = cross(aNormal, vec3(0.0, 1.0, 0.0));

        tangent = length(c1) > length(c2) ? c1 : c2;
        tangent = normalize(tangent);
        bitangent = normalize(cross(aNormal, tangent));

        vec3 T = tangent;
        vec3 B = bitangent;
        vec3 N = normalize(aNormal);
        TBN = mat3(T, B, N);
}
