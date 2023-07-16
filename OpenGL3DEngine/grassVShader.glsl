
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 3) in vec4 aColor; 
  
out vec3 objectColor; // output a color to the fragment shader
out float opacity;
out vec3 worldPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    worldPos = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    Normal = aNormal;
    objectColor = aColor.xyz;
    opacity = aColor.w;
}       