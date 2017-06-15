#version 120

// Pass the matrices in as uniforms
uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform vec3 camera_position;
uniform mat4 light_space_matrix;

// World position is part of the VBO
attribute vec3 world_position;
attribute vec3 normal;
attribute vec3 color;
attribute vec2 texture_position;

// Outputs
varying vec3 tint;
varying vec3 world_pos;
varying vec4 fragment_light_space_position;
varying mat3 TBN;

void main() {
    vec3 tangent;
    vec3 bitangent;

    vec3 c1 = cross(normal, vec3(0.0, 0.0, 1.0));
    vec3 c2 = cross(normal, vec3(0.0, 1.0, 0.0));

    if (length(c1)>length(c2)) {
        tangent = c1;
    } else {
        tangent = c2;
    }
    tangent = normalize(tangent);
    bitangent = normalize(cross(normal, tangent));

    vec3 T = tangent;
    vec3 B = bitangent;
    vec3 N = normalize(normal);
    TBN = mat3(T, B, N);

    vec4 position = gl_Vertex;
    position.xyz += world_position.xzy;
    gl_Position = projection_matrix * (view_matrix * position);
    tint = color/2.0;
    if (world_position.z < camera_position.z) {
        float darken = (camera_position.z - world_position.z) * 0.05f;
        tint.rgb -= darken;
    }
    gl_TexCoord[0] = vec4(texture_position, 0.0, 0.0);
    gl_TexCoord[1] = vec4(texture_position.x + 0.5f, texture_position.y, 0.0, 0.0);
    world_pos = world_position;
    fragment_light_space_position = light_space_matrix * position;
}
