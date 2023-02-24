
#version 330 core
out vec4 FragColor;  
in vec3 ourColor;
in vec4 outPos;
  
void main()
{
    FragColor = vec4(ourColor.x, ourColor.y, ourColor.z, 1.0);
}