
#version 330 core
out vec4 FragColor;  
in vec3 ourColor;
in vec4 outPos;
in vec2 TexCoord;

uniform sampler2D ourTexture;
  
void main()
{
    FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1);
}