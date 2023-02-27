
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normals;
layout (location = 2) in vec2 aTexCoord;
<<<<<<< Updated upstream
layout (location = 3) in vec3 aColor; 
=======
layout (location = 3) in vec3 aColor;
>>>>>>> Stashed changes
  
out vec3 ourColor; // output a color to the fragment shader
out vec4 outPos;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    outPos = projection * view * model * vec4(aPos, 1.0);
    gl_Position = outPos;
    ourColor = aColor;
    TexCoord = aTexCoord;
}       