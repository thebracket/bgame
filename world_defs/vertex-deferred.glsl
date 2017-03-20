#version 120

varying vec3 normal;

// Outdoor rendering
void main()
{
    // Transform the normal and normalize (heh) it
    normal = normalize(gl_NormalMatrix * gl_Normal);

    // Now we get the light direction (in eye space)
    vec3 lightDir = normalize(vec3(-gl_LightSource[0].position));

    // Now we get the dot product between the light and the vertex
    float NdotL = max(dot(normal, lightDir), 0.0);
    vec4 diffuse = gl_LightSource[0].diffuse * gl_Color;

    // Ambient term
    vec4 ambient = gl_LightSource[0].ambient;

    // Transforming The Vertex
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

    // Pass the color through because we use it for colored textures
    //gl_FrontColor = vec4(NdotL, NdotL, NdotL, 1.0);
    //gl_FrontColor = vec4(normal.x, normal.y, normal.z, 1.0);

    gl_FrontColor = vec4(
                (diffuse.r * NdotL) + ambient.r,
                (diffuse.g * NdotL) + ambient.g,
                (diffuse.b * NdotL) + ambient.b,
                1.0);

    // Project the texture
    gl_TexCoord[0] = gl_MultiTexCoord0;
}