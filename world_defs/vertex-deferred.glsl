#version 120

uniform vec3 light_position;
uniform vec3 light_ambient;
uniform vec3 light_diffuse;
uniform mat4 projection_matrix;
uniform mat4 view_matrix;
varying vec3 normal, ambient;
varying vec4 diffuse;
varying vec3 lightDir;
attribute vec3 screen_index;
varying vec3 si;

// Outdoor rendering
void main()
{
    // Transforming The Vertex
    gl_Position = projection_matrix * (view_matrix * gl_Vertex);

    // Transform the normal and normalize (heh) it
    normal = normalize(gl_Normal * gl_NormalMatrix);

    // Now we get the light direction (in eye space)
    vec3 light_position_transformed = light_position * gl_NormalMatrix;
    lightDir = normalize((vec3(gl_Vertex) * gl_NormalMatrix)-light_position_transformed);

    // Now we get the dot product between the light and the vertex
    diffuse = vec4(light_diffuse, 1.0f) * gl_Color;

    // Pass the color through because we use it for colored textures
    gl_FrontColor = gl_Color;
    ambient = light_ambient;
    si = screen_index.xyz;

    // Project the texture
    gl_TexCoord[0] = gl_MultiTexCoord0;
}