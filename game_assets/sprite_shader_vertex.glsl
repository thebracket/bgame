#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 texture_info;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aColor;
layout (location = 4) in vec3 world_in;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;

out vec3 tex_pos;
out vec3 world_pos;
out vec3 base_normal;
out vec3 tint;

mat4 translate(float x, float y, float z){
    return mat4(
        vec4(1.0, 0.0, 0.0, 0.0),
        vec4(0.0, 1.0, 0.0, 0.0),
        vec4(0.0, 0.0, 1.0, 0.0),
        vec4(x,   y,   z,   1.0)
    );
}

void main()
{
    // We want to rotate to face the camera
    mat4 ModelView = view_matrix * translate(world_in.x, world_in.y, world_in.z);

    // Column 0:
    ModelView[0][0] = 1;
    ModelView[0][1] = 0;
    ModelView[0][2] = 0;

    // Column 1:
    ModelView[1][0] = 0;
    ModelView[1][1] = 1;
    ModelView[1][2] = 0;

    // Column 2:
    ModelView[2][0] = 0;
    ModelView[2][1] = 0;
    ModelView[2][2] = 1;

    vec4 position = vec4(aPos, 1.0); // At this point we're in -0.5 to 0.5 space
    gl_Position = projection_matrix * (ModelView * position);

    //gl_Position = projection_matrix * (view_matrix * vec4(aPos, 1.0));
    tex_pos = texture_info;
    world_pos = aPos;
    base_normal = normalize(aNormal);
    tint = aColor;
}
