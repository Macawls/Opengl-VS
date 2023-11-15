R"glsl(
#version 330 core

// constants
// not going to implement materials due to scope
#define SPECULAR 0.5
#define SHININESS 32.0
#define AMBIENT 0.1
#define POINT_LIGHT_COUNT 5

in vec2 TexCoord;
in vec3 FragColor;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragOutput;

uniform sampler2D texture1;
uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 viewPosition;
uniform PointLight pointLights[POINT_LIGHT_COUNT];

// Types of Lights

// Directional
struct DirectionalLight 
{
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// Point
struct PointLight 
{
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// Spot
struct SpotLight 
{
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

// func declarations
vec3 ComputeDirectional(DirectionalLight light, vec3 normal, vec3 viewDirection);
vec3 ComputePoint(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDirection);
vec3 ComputeSpot(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDirection);


void main()
{
    vec3 normal = normalize(Normal);
    vec3 viewDirection = normalize(viewPosition - FragPos);
    
    // directional
    vec3 result = CalcDirLight(dirLight, normal, viewDirection);
    // point lights
    for(int i = 0; i < POINT_LIGHT_COUNT; i++)
    {
        result += CalcPointLight(pointLights[i], normal, FragPos, viewDirection);    
    }
     /*
    // phase 3: spot light
    result += CalcSpotLight(spotLight, normal, FragPos, viewDirection); 
     */   

    FragOutput = vec4(result, 1.0);
}


// Function Implementations

vec3 ComputeDirectional(DirectionalLight light, vec3 normal, vec3 viewDirection)
{
    vec3 lightDirection = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDirection), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDirection, reflectDir), 0.0), SHININESS);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(texture1, TexCoords)) * AMBIENT;
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture1, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(texture1, TexCoords)) * SPECULAR;
    
    return (ambient + diffuse + specular) * FragColor;
}

vec3 ComputePoint(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDirection)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    // specular 
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDirection, reflectDir), 0.0), SHININESS);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * vec3(texture(texture1, TexCoords)) * AMBIENT;
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture1, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(texture1, TexCoords)) * SPECULAR;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    return (ambient + diffuse + specular) * FragColor;
}

vec3 ComputeSpot(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDirection, vec2 TexCoords)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse 
    float diff = max(dot(normal, lightDir), 0.0);
    // specular 
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDirection, reflectDir), 0.0), SHININESS);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(texture1, TexCoords)) * AMBIENT;
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture1, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(texture1, TexCoords)) * SPECULAR;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular) * FragColor;
}
)glsl"