#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 texture_info;
layout (location = 2) in vec3 aNormal;
//layout (location = 3) in vec3 aTangent;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform vec3 camera_position;
uniform mat3 normalMatrix;

out vec3 tex_pos;
out vec3 world_pos;
out vec3 TangentViewPos;
out vec3 TangentFragPos;

void main()
{
    tex_pos = texture_info;
    world_pos = aPos;
    vec3 aTangent = aNormal.x == 0.0 ? vec3(1.0, 0.0, 0.0) : vec3(0.0, 1.0, 0.0);

    vec3 T = normalize(normalMatrix * aTangent);
    vec3 N = normalize(normalMatrix * aNormal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);

    mat3 TBN = transpose(mat3(T, B, N));
    TangentViewPos = TBN * camera_position;
    TangentFragPos = TBN * aPos;

    gl_Position = projection_matrix * view_matrix * vec4(aPos, 1.0);
}
