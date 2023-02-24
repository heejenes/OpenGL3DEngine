
#version 330 core
out vec4 FragColor;  
in vec3 ourColor;
in vec4 outPos;
in vec2 TexCoord;

uniform sampler2D ourTexture;
  
void main()
{
    FragColor = texture(ourTexture, TexCoord) * vec4(0.5, 0.5, 0.5, 1.0) * vec4(outPos.z, 1, 1, 1);
}