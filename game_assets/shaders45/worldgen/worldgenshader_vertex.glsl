#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 texture_info;
layout (location = 2) in float rivers;

uniform mat4 projection_mat;
uniform mat4 view;
uniform mat4 model;

out VS_OUT {
    out vec3 texture_position;
    out vec3 frag_pos;
    out mat3 TBN;
    out float river;
} vs_out;

void main()
{
    vec4 pos = vec4(aPos, 1.0);
    gl_Position = projection_mat * view * model * pos;
    vec3 normal = normalize(mat3(transpose(inverse(model))) * aPos);
    vs_out.frag_pos = (model * pos).xyz;
    vs_out.texture_position = texture_info;

    vec3 tangent;
    vec3 bitangent;

    vec3 c1 = cross(normal, vec3(0.0, 0.0, 1.0));
    vec3 c2 = cross(normal, vec3(0.0, 1.0, 0.0));

    tangent = length(c1) > length(c2) ? c1 : c2;
    tangent = normalize(tangent);
    bitangent = normalize(cross(normal, tangent));

    vec3 T = tangent;
    vec3 B = bitangent;
    vec3 N = normalize(normal);
    vs_out.TBN = mat3(T, B, N);

    vs_out.river = rivers;
}
