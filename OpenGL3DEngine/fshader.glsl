
#version 330 core
out vec4 FragColor;  
in vec3 objectColor;
in vec2 TexCoord;

in vec3 Normal;

uniform sampler2D ourTexture;

uniform vec3 ambientColor;
uniform vec3 emitterColor;
uniform vec3 emitterPos;
in vec3 worldPos;
  
void main()
{
    vec3 N = normalize(Normal);
    vec3 lightDir = normalize(emitterPos - worldPos);
    // diffuse color is 0 if angle is past 90 degrees
    float diff = max(dot(N, lightDir), 0.0);
    vec3 diffuse = diff * emitterColor;
    vec3 lightResult = (ambientColor + diffuse) * objectColor;
    FragColor = texture(ourTexture, TexCoord) * vec4(lightResult, 1.0);
}