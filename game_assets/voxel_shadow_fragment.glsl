#version 330 core

uniform vec3 lightPos;
uniform float far_plane;

in vec3 world_pos;

void main() {
    // get distance between fragment and light source
    float lightDistance = abs(distance(world_pos.xyz, lightPos));
    
    // map to [0;1] range by dividing by far_plane
    lightDistance = lightDistance / far_plane;
    
    // write this as modified depth
    gl_FragDepth = lightDistance;
}