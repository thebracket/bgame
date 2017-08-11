#version 330 core

uniform sampler2DArray textureArray;
uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform mat4 light_projection_matrix;
uniform mat4 light_view_matrix;

in vec3 tex_pos;
in vec3 world_pos;
in mat3 TBN;

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gAlbedo;
layout (location = 3) out vec3 gLightPos;
layout (location = 4) out vec3 gLightCol;

out vec4 FragColor;

float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir)
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
    //float bias = 0.005;
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    float shadow = currentDepth - bias > closestDepth  ? 0.0 : 1.0;

    return shadow;
}

void main() {
    float gamma = 2.2;
    vec3 base_color = pow(texture(textureArray, tex_pos).rgb, vec3(gamma));
    gAlbedo = base_color;
    gPosition = vec3(world_pos.x/256.0, world_pos.y/256.0, world_pos.z/256.0);

    vec3 norm = texture(textureArray, vec3(tex_pos.x, tex_pos.y, tex_pos.z+1)).rgb;
    norm = normalize(norm * 2.0 - 1.0);
    norm = normalize(TBN * norm);
    gNormal = norm;

    vec4 FragPosLightSpace = light_projection_matrix * (light_view_matrix * vec4(world_pos, 1.0));
    vec3 lightDir = normalize(lightPos - world_pos);
    float shadow = ShadowCalculation(FragPosLightSpace, norm, lightDir);
    gLightPos = shadow==0.0 ? vec3(0.0) : vec3(lightPos.x/256.0, lightPos.y/256.0, lightPos.z/256.0);
    gLightCol = shadow==0.0 ? vec3(0.0) : lightColor;
}