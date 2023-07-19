
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec4 aColor; 
layout (location = 4) in mat4 instanceMatrix;
layout (location = 8) in vec2 windDir;
layout (location = 9) in float windStrength;
  
out vec3 objectColor; // output a color to the fragment shader
out float opacity;
out vec3 worldPos;
out vec3 localPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    localPos = aPos;
    worldPos = vec3(instanceMatrix * vec4(aPos, 1.0));
    gl_Position = projection * view * instanceMatrix * vec4(aPos, 1.0);
    Normal = aNormal;
    objectColor = aColor.xyz;
    opacity = aColor.w;
}       