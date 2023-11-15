R"glsl(
#version 330 core
// N.B define before parsing

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

// constants
// not going to implement materials due to scope
#define SPECULAR 0.5
#define SHININESS 32.0
#define AMBIENT 0.1
#define POINT_LIGHT_COUNT 4

in vec2 TexCoord;
in vec3 FragColor;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragOutput;

// uniforms
uniform sampler2D texture1;
uniform vec3 viewPosition;
uniform DirectionalLight directionalLight;
uniform PointLight pointLights[POINT_LIGHT_COUNT];

// func declarations
vec3 ComputeDirectional(DirectionalLight light, vec3 normal, vec3 viewDirection);
vec3 ComputePoint(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDirection);
vec3 ComputeSpot(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDirection);


void main()
{
    vec3 normal = normalize(Normal);
    vec3 viewDirection = normalize(viewPosition - FragPos);
    
    // directional
    vec3 result = ComputeDirectional(directionalLight, normal, viewDirection);
    // point lights
    for(int i = 0; i < POINT_LIGHT_COUNT; i++)
    {
        result += ComputePoint(pointLights[i], normal, FragPos, viewDirection);    
    }
     /*
    // phase 3: spot light
    result += CalcSpotLight(spotLight, normal, FragPos, viewDirection); 
     */   

    FragOutput = vec4(result, 1.0);
}


// Implementations

vec3 ComputeDirectional(DirectionalLight light, vec3 normal, vec3 viewDirection)
{
    vec3 lightDirection = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDirection), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDirection, reflectDir), 0.0), SHININESS);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(texture1, TexCoord)) * AMBIENT;
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture1, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(texture1, TexCoord)) * SPECULAR;
    
    return (ambient + diffuse + specular) * FragColor;
}

vec3 ComputePoint(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDirection)
{
    vec3 lightDirection = normalize(light.position - fragPos);
    // diffuse
    float diff = max(dot(normal, lightDirection), 0.0);
    // specular 
    vec3 reflectDir = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDirection, reflectDir), 0.0), SHININESS);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * vec3(texture(texture1, TexCoord)) * AMBIENT;
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture1, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(texture1, TexCoord)) * SPECULAR;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    return (ambient + diffuse + specular) * FragColor;
}

vec3 ComputeSpot(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDirection)
{
    vec3 lightDirection = normalize(light.position - fragPos);
    // diffuse 
    float diff = max(dot(normal, lightDirection), 0.0);
    // specular 
    vec3 reflectDir = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDirection, reflectDir), 0.0), SHININESS);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // spotlight intensity
    float theta = dot(lightDirection, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(texture1, TexCoord)) * AMBIENT;
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture1, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(texture1, TexCoord)) * SPECULAR;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular) * FragColor;
}
)glsl"