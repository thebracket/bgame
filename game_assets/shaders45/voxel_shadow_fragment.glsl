#version 430 core

in vec3 wp;

uniform vec3 lightPos;
uniform float far_plane;

void main() 
{
    // get distance between fragment and light source
    float lightDistance = abs(distance(wp.xyz, lightPos));

    // map to [0;1] range by dividing by far_plane
    lightDistance = lightDistance / far_plane;
    
    // write this as modified depth
    gl_FragDepth = lightDistance;
}