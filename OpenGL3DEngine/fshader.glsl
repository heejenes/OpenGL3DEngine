
#version 330 core
out vec4 FragColor;  
in vec3 ourColor;
in vec4 outPos;
in vec2 TexCoord;

uniform sampler2D ourTexture;

uniform vec3 emitterColor;
  
void main()
{
    FragColor = texture(ourTexture, TexCoord) * vec4(emitterColor * ourColor, 1.0);
    //FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1);
}