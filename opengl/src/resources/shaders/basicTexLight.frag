R"glsl(
#version 330 core

in vec2 TexCoord;
in vec3 FragColor;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragOutput;

uniform sampler2D texture1;
uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 viewPosition;

void main()
{
    // coefficients
    float specularStrength = 0.5;
    float ambientStrength = 0.1;

    // Ambient contribution
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse contribution
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPosition - FragPos);
    vec3 diffuse = max(dot(norm, lightDir), 0.0) * lightColor;

    // Specular contribution
    vec3 viewDir = normalize(viewPosition - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;   
    
    // Combine ambient, diffuse, and specular
    vec3 result = (ambient + diffuse + specular) * FragColor;

    // Apply texture and set final output color
    FragOutput = texture(texture1, TexCoord) * vec4(result, 1.0);
}
)glsl"