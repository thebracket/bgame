#version 430 core
layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

struct light_data_t {
    mat4 shadowMatrices[6];
    vec3 lightPos;
    float far_plane;
};

layout (std140) uniform light_data
{
    light_data_t li[100];
};

out vec4 FragPos; // FragPos from GS (output per emitvertex)
uniform uint light_index;

void main()
{
    for(int face = 0; face < 6; ++face)
    {
        gl_Layer = face; // built-in variable that specifies to which face we render.
        for(int i = 0; i < 3; ++i) // for each triangle's vertices
        {
            FragPos = gl_in[i].gl_Position;
            gl_Position = li[light_index].shadowMatrices[face] * FragPos;
            EmitVertex();
        }    
        EndPrimitive();
    }
}  