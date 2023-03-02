
#version 330 core
out vec4 FragColor;  
in vec3 objectColor;
in vec2 TexCoord;

in vec3 Normal;
in vec3 worldPos;

uniform sampler2D ourTexture;

uniform vec3 ambientColor;
uniform vec3 emitterColor;
uniform vec3 emitterPos;
uniform vec3 viewPos;
  
void main()
{

    // diffuse
    vec3 N = normalize(Normal);
    vec3 lightDir = normalize(emitterPos - worldPos);
    // diffuse color is 0 if angle is past 90 degrees
    float diff = max(dot(N, lightDir), 0.0);
    vec3 diffuse = diff * emitterColor;
    /////////////////////////////////////

    // specular
    float specularStrength = 0.5;
    float shininess = 32;
    vec3 viewDir = normalize(viewPos - worldPos);
    vec3 reflectDir = reflect(-lightDir, N);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * emitterColor;  
    ///////////////////////////////////////

    vec3 lightResult = (ambientColor + diffuse + specular) * objectColor;
    FragColor = texture(ourTexture, TexCoord) * vec4(lightResult, 1.0);
}