#version 430 core
layout (triangles, invocations = 6) in;
layout (triangle_strip, max_vertices=18) out;

out vec4 FragPos; // FragPos from GS (output per emitvertex)

uniform mat4 shadowMatrices[6];

in mat4 model_view_matrix[3];
in vec3 world_pos[3];
out vec3 wp;

void main()
{
        for(int i = 0; i < 3; ++i) // for each triangle's vertices
        {
            //FragPos = gl_in[i].gl_Position;
            gl_Layer = gl_InvocationID; // built-in variable that specifies to which face we render.
            gl_Position = shadowMatrices[gl_InvocationID] * model_view_matrix[i] * gl_in[i].gl_Position;
            wp = world_pos[i];
            EmitVertex();
        }    
        EndPrimitive();
}  