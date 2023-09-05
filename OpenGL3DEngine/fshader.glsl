
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
in vec2 TexCoord;

in float opacity;

in vec3 Normal;
in vec3 worldPos;

uniform sampler2D ourTexture;

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
    if (light.lightDir.w == 0.0)
        lightDir3 = normalize(light.position - worldPos);
    else  // if sun light
        lightDir3 = normalize(light.lightDir.xyz);
    // diffuse color is 0 if angle is past 90 degrees
    float diff = max(dot(N, lightDir3), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
    /////////////////////////////////////

    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - worldPos);
    vec3 reflectDir = reflect(-lightDir3, N);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);  
    ///////////////////////////////////////

    // attenuation
    if (light.lightDir.w == 0.0) {
        float distance;
        float attenuation; 
        distance = length(light.position - worldPos);
        attenuation = 1.0 / (light.constant + light.linear * distance + 
            light.quadratic * (distance * distance)); 
            
        ambient *= attenuation;  
        diffuse *= attenuation;
        specular *= attenuation;
    }
    ///////////////////////////////////////

    // fog
    float viewDist = distance(viewPos, worldPos.xyz);
    float fogCo = max(0.1f, 1.f - 0.007f * viewDist); 

    vec3 lightResult = ambient + diffuse + specular;
    FragColor = texture(ourTexture, TexCoord) * vec4(lightResult * fogCo, 1.0) * vec4(objectColor, opacity);
}