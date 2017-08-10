#version 330 core

layout (location = 3) out vec3 gLightPos;
layout (location = 4) out vec3 gLightCol;

in vec4 FragPosLightSpace;

uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 lightColor;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float bias = 0.005;
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

    return shadow;
}

void main()
{
    float shadow = ShadowCalculation(FragPosLightSpace);
    if (shadow == 0.0) {
        discard;
    } else {
        gLightPos = vec3(lightPos.x/256.0, lightPos.y/256.0, lightPos.z/256.0);
        gLightCol = lightColor;
    }
}