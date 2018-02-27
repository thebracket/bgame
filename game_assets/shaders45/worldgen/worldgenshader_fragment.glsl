#version 430 core
out vec4 FragColor;

uniform sampler2DArray textureArray;

in VS_OUT {
    in vec3 texture_position;
    in vec3 frag_pos;
    in mat3 TBN;
    in float river;
} fs_in;

void main()
{
    vec3 norm = texture(textureArray, vec3(fs_in.texture_position.x, fs_in.texture_position.y, fs_in.texture_position.z+1)).rgb;
    norm = normalize(norm * 2.0 - 1.0);
    norm = normalize(fs_in.TBN * norm);

    vec3 base_color = texture(textureArray, fs_in.texture_position).rgb;
    if (fs_in.river > 0.0) base_color.b = 1.0f;
    vec3 light_pos = vec3(1.0f, 100.0f, 0.0f);
    vec3 ambient = base_color * 0.3;

    vec3 lightDir = normalize(light_pos - fs_in.frag_pos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = base_color * diff;

    float specularStrength = 0.5;
    vec3 viewDir = normalize(light_pos - fs_in.frag_pos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = vec3(specularStrength * spec);

    FragColor = vec4(ambient + diffuse + specular, 1.0);
}
