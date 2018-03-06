#version 430 core

in vec3 wp;

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

void main() 
{
    // get distance between fragment and light source
    float lightDistance = abs(distance(wp.xyz, li[light_index].lightPos));

    // map to [0;1] range by dividing by far_plane
    lightDistance = lightDistance / li[light_index].far_plane;
    
    // write this as modified depth
    gl_FragDepth = lightDistance;
}