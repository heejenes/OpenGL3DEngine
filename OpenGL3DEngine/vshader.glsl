
#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec3 aColor; // the color variable has attribute position 1
layout (location = 2) in vec2 aTexCoord;
  
out vec3 ourColor; // output a color to the fragment shader
out vec4 outPos;
out vec2 TexCoord;

void main()
{
    outPos = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    gl_Position = outPos;
    ourColor = vec3(1 * aColor.x, 1* aColor.y, 1 * aColor.z);
    TexCoord = aTexCoord;
}       