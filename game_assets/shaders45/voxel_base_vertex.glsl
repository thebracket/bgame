#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec3 instancePos;
layout (location = 4) in vec4 instanceRotation;
layout (location = 5) in vec3 instanceTint;

layout (std140) uniform camera_data
{
    mat4 projection_matrix;
    mat4 view_matrix;
    mat4 proj_view_matrix;
    vec3 camera_position;
};

uniform float texSize;

out VS_OUT {
    vec3 world_pos;
    vec3 base_normal;
    vec3 color;
    vec3 tint;
} vs_out;

mat4 translate(float x, float y, float z){
    return mat4(
        vec4(1.0, 0.0, 0.0, 0.0),
        vec4(0.0, 1.0, 0.0, 0.0),
        vec4(0.0, 0.0, 1.0, 0.0),
        vec4(x,   y,   z,   1.0)
    );
}

mat4 rotationMatrix(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;

    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}

void main()
{
    vec3 shunk_pos = (aPos - texSize/2.0) / texSize;
    vec4 position = vec4(shunk_pos, 1.0);

    mat4 rotation = rotationMatrix(instanceRotation.xyz, instanceRotation.w);
    mat4 translation = translate(instancePos.x, instancePos.y, instancePos.z);
    mat4 model_view_matrix = instanceRotation.w > 0 ? view_matrix * translation * rotation : view_matrix * translation;
    gl_Position = projection_matrix * model_view_matrix * position;
    vs_out.world_pos = shunk_pos + instancePos;    
    vs_out.base_normal = instanceRotation.w > 0 ? normalize(rotation * vec4(aNormal, 0.0)).xyz : normalize(aNormal);
    vs_out.color = aColor;
    vs_out.tint = instanceTint;
}
