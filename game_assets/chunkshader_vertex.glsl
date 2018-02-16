#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 texture_info;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBiTangent;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform vec3 camera_position;

out vec3 tex_pos;
out vec3 world_pos;
out vec3 base_normal;
out vec3 TangentViewPos;
out vec3 TangentFragPos;

const mat4 model = mat4(1.0);

void main()
{
    tex_pos = texture_info;
    world_pos = aPos;

    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 N = normalize(normalMatrix * aNormal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);

    mat3 TBN = transpose(mat3(T, B, N));
    TangentViewPos = TBN * camera_position;
    TangentFragPos = TBN * aPos;

    base_normal = normalize(aNormal);
    gl_Position = projection_matrix * view_matrix * model * vec4(aPos, 1.0);
}
