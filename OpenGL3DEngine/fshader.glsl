
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
  
void main()
{
    vec3 N = normalize(Normal);
    vec3 lightDir = normalize(emitterPos - worldPos);
    // diffuse color is 0 if angle is past 90 degrees
    vec3 diffuse = max(dot(N, lightDir), 0.0) * emitterColor;
    vec3 lightResult = (ambientColor + diffuse) * objectColor;
    FragColor = texture(ourTexture, TexCoord) * vec4(lightResult, 1.0);
}