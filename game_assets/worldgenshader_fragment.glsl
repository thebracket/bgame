#version 330 core
out vec4 FragColor;

uniform sampler2DArray textureArray;

in vec3 texture_position;
in vec3 frag_pos;
in mat3 TBN;

void main()
{
    vec3 norm = texture(textureArray, vec3(texture_position.x, texture_position.y, texture_position.z+1)).rgb;
    norm = normalize(norm * 2.0 - 1.0);
    norm = normalize(TBN * norm);

    vec3 base_color = texture(textureArray, texture_position).rgb;
    vec3 light_pos = vec3(1.0f, 100.0f, 0.0f);
    vec3 ambient = base_color * 0.1;

    vec3 lightDir = normalize(light_pos - frag_pos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = base_color * diff;

    float specularStrength = 0.5;
    vec3 viewDir = normalize(light_pos - frag_pos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = vec3(specularStrength * spec);

    FragColor = vec4(ambient + diffuse + specular, 1.0);
}
