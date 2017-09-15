#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D hdr_tex;
//uniform sampler2D blur_tex;

void main()
{
    const float gamma = 2.2;
    vec3 hdrColor = texture(hdr_tex, TexCoords).rgb;
    //vec3 bloomColor = texture(blur_tex, TexCoords).rgb;
    //hdrColor += bloomColor;

    vec3 mapped = hdrColor / (hdrColor + vec3(1.0));
    mapped = pow(mapped, vec3(1.0 / gamma)); // Apply gamma correction

    FragColor = vec4(mapped, 1.0);
}
