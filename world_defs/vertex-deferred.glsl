#version 120

uniform vec3 light_position;
uniform vec3 light_ambient;
uniform vec3 light_diffuse;
//varying vec3 normal;

// Outdoor rendering
void main()
{
    // Transforming The Vertex
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

    // Transform the normal and normalize (heh) it
    vec3 normal = normalize(gl_Normal * gl_NormalMatrix);

    // Now we get the light direction (in eye space)
    vec3 light_position_transformed = light_position * gl_NormalMatrix;
    vec3 lightDir = normalize((vec3(gl_Vertex) * gl_NormalMatrix)-light_position_transformed);

    // Now we get the dot product between the light and the vertex
    float NdotL = max(dot(normal, lightDir), 0.0);
    vec4 diffuse = vec4(light_diffuse, 1.0f) * gl_Color;

    // Pass the color through because we use it for colored textures
    //gl_FrontColor = vec4(NdotL, NdotL, NdotL, 1.0);
    //gl_FrontColor = vec4(normal.x, normal.y, normal.z, 1.0);

    gl_FrontColor = vec4(
                (diffuse.r * NdotL) + light_ambient.r,
                (diffuse.g * NdotL) + light_ambient.g,
                (diffuse.b * NdotL) + light_ambient.b,
                1.0);

    // Project the texture
    gl_TexCoord[0] = gl_MultiTexCoord0;
}