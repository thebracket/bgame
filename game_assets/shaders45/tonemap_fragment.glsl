#version 430 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D hdr_tex;
uniform sampler2D blur_tex;

vec3 Uncharted2Tonemap(vec3 x)
{
    float A = 0.15;
    float B = 0.50;
    float C = 0.10;
    float D = 0.20;
    float E = 0.02;
    float F = 0.30;

    return ((x*(A*x+C*B)+D*E)/(x*(A*x+B)+D*F))-E/F;
}

vec3 expose(vec3 x, float exposure) {
    return vec3(1.0) - exp(-x * exposure);
}

void main()
{
    const float gamma = 2.2;
    vec3 hdrColor = texture(hdr_tex, TexCoords).rgb;
    vec3 bloomColor = texture(blur_tex, TexCoords, 5).rgb;
    hdrColor += bloomColor;

    //vec3 mapped = hdrColor / (hdrColor + vec3(1.0));
    //vec3 mapped = Uncharted2Tonemap(hdrColor);
    vec3 lowest_detail = texture(hdr_tex, vec2(0.5,0.5), 10).rgb;
    float AverageBrightness = clamp( max(max(lowest_detail.r, lowest_detail.g), lowest_detail.b), 0.3, 0.7 );
    float exposure = 0.5 / AverageBrightness;

    vec3 mapped = expose(hdrColor, exposure); // Disable tone-mapping
    mapped = pow(mapped, vec3(1.0 / gamma)); // Apply gamma correction

    FragColor = vec4(mapped, 1.0);
}
