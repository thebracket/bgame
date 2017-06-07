#version 120

uniform sampler2D albedo_tex;
uniform sampler2D position_tex;
uniform sampler2D normal_tex;
uniform sampler2D shadow_map;

uniform vec3 cameraPosition;

void main() {
    vec4 base_color = texture2D( albedo_tex, gl_TexCoord[0].xy );
    vec4 position = texture2D( position_tex, gl_TexCoord[0].xy );
    vec4 normal = texture2D( normal_tex, gl_TexCoord[0].xy );

    vec3 light = cameraPosition;
    vec3 lightDir = light - position.xyz;

    normal = normalize(normal);
    lightDir = normalize(lightDir);

    vec3 eyeDir = normalize(cameraPosition-position.xyz);
    vec3 vHalfVector = normalize(lightDir.xyz + eyeDir);

    gl_FragColor = max(dot(normal.xyz,lightDir),0) * base_color +
                       pow(max(dot(normal.xyz,vHalfVector),0.0), 100) * 1.5;
}