#version 120

uniform sampler2D my_color_texture;
varying vec3 normal;
varying vec3 ambient;
varying vec4 diffuse;
varying vec3 lightDir;

// Outdoor rendering
void main()
{
    float NdotL = max(dot(normal, lightDir), 0.0);

    vec4 color = vec4(
         (diffuse.r * NdotL) + ambient.r,
         (diffuse.g * NdotL) + ambient.g,
         (diffuse.b * NdotL) + ambient.b,
         1.0);

    gl_FragColor = texture2D(my_color_texture, gl_TexCoord[0].st) * color * gl_Color;
}