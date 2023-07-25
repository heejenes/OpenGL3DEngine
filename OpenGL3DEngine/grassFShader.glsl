#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
}; 

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec4 lightDir;
    
    float constant;
    float linear;
    float quadratic;
};

out vec4 FragColor;  
in vec3 objectColor;

in float opacity;

in vec3 Normal;
in vec4 worldPos;
in vec4 localPos;

uniform Material material;
uniform Light light;

uniform vec3 viewPos;
  
void main()
{
    // ambient
    vec3 ambient = material.ambient * light.ambient;

    // diffuse
    vec3 N = normalize(Normal);
    vec3 lightDir3;
    // if point light
    if (light.lightDir.w == 0.0f)
        lightDir3 = normalize(light.position - worldPos.xyz);
    else  // if sun light
        lightDir3 = normalize(light.lightDir.xyz);
    // diffuse color is 0 if angle is past 90 degrees
    float diff = max(dot(N, lightDir3), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
    /////////////////////////////////////

    // specular
    float specularStrength = 0.1f;
    vec3 viewDir = normalize(viewPos - worldPos.xyz);
    vec3 reflectDir = reflect(-lightDir3, N);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);  
    ///////////////////////////////////////

    // attenuation
    if (light.lightDir.w == 0.0f) {
        float distance;
        float attenuation; 
        distance = length(light.position - worldPos.xyz);
        attenuation = 1.0f / (light.constant + light.linear * distance + 
            light.quadratic * (distance * distance)); 
            
        ambient *= attenuation;  
        diffuse *= attenuation;
        specular *= attenuation;
    }
    ///////////////////////////////////////

    vec3 lightResult =  ambient + diffuse + specular;
    vec3 finalColor = objectColor * min(max(0.2f, localPos.y - 0.5f), 0.8f);
    FragColor = vec4(lightResult, 1.0f) * vec4(finalColor, opacity);
}