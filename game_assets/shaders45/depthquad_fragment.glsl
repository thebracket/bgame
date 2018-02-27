#version 430 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D depthMap;

void main()
{
    vec3 depthValue = texture(depthMap, TexCoords).rgb;
    FragColor = vec4(depthValue, 1.0);
}
