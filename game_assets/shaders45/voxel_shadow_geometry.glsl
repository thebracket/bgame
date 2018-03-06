#version 430 core
layout (triangles, invocations = 6) in;
layout (triangle_strip, max_vertices=18) out;

out vec4 FragPos; // FragPos from GS (output per emitvertex)

struct light_data_t {
    mat4 shadowMatrices[6];
    vec3 lightPos;
    float far_plane;
};

layout (std140) uniform light_data
{
    light_data_t li[100];
};

uniform uint light_index;

in mat4 model_view_matrix[3];
in vec3 world_pos[3];
out vec3 wp;

void main()
{
        for(int i = 0; i < 3; ++i) // for each triangle's vertices
        {
            //FragPos = gl_in[i].gl_Position;
            gl_Layer = gl_InvocationID; // built-in variable that specifies to which face we render.
            gl_Position = li[light_index].shadowMatrices[gl_InvocationID] * model_view_matrix[i] * gl_in[i].gl_Position;
            wp = world_pos[i];
            EmitVertex();
        }    
        EndPrimitive();
}  