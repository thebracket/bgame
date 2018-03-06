#version 430
#extension GL_ARB_shader_draw_parameters : enable

layout (location = 0) in vec3 aPos;

const float texSize = 32.0;

struct InstanceData {
    float x;
    float y;
    float z;
    float axis1;
    float axis2;
    float axis3; 
    float rot_angle;
    float tint_r;
    float tint_g;
    float tint_b;
};

layout (std430, binding=2) buffer instance_data
{ 
    InstanceData i[];
} instancedData;

in int GeomInstance;

mat4 translate(float x, float y, float z){
    return mat4(
        vec4(1.0, 0.0, 0.0, 0.0),
        vec4(0.0, 1.0, 0.0, 0.0),
        vec4(0.0, 0.0, 1.0, 0.0),
        vec4(x,   y,   z,   1.0)
    );
}

out mat4 model_view_matrix;
out vec3 world_pos;

void main()
{
	InstanceData instance = instancedData.i[gl_BaseInstanceARB + gl_InstanceID];
    vec3 instancePos = vec3(instance.x, instance.y, instance.z);

    vec3 shunk_pos = (aPos - texSize/2.0) / texSize;
    vec4 position = vec4(shunk_pos, 1.0);

    mat4 translation = translate(instancePos.x, instancePos.y, instancePos.z);
    model_view_matrix = translation;
    gl_Position = position;
    world_pos = shunk_pos + instancePos;
}
