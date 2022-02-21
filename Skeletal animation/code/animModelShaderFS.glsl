#version 330 core
out vec4 FragColor;

struct PointLight {
    vec3 position;
    vec3 color;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// #define amount of lights
const int NR_POINT_LIGHTS = 3;

in vec3 FragPos;
in vec3 Normal; 
in vec2 TexCoords;   

uniform sampler2D texture_diffuse1;
uniform vec3 viewPos; 
uniform PointLight pointLights[NR_POINT_LIGHTS];

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, sampler2D tex);

void main()
{   
    // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = vec3(0.0, 0.0, 0.0);
    for (int i = 0; i < NR_POINT_LIGHTS; i++) {
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir, texture_diffuse1);
    }

    FragColor =  vec4(result, 1.0f);

// --------- DEBUG --------
//    FragColor.rgb = norm;
//    FragColor.a = 1.0f;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, sampler2D tex)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 specular = vec3(0.0f);
    if (diff > 0.0) 
    {
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0f);
        specular = light.specular * spec * light.color;  
    }
    // attenuation
    float argDistance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * argDistance + light.quadratic * (argDistance * argDistance));    
    // combine results
    vec3 ambient = light.ambient * light.color * vec3(texture(tex, TexCoords));
    vec3 diffuse = light.diffuse * diff * light.color * vec3(texture(tex, TexCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}