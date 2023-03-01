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
    FragColor = vec4(objectColor, 1.0);
}