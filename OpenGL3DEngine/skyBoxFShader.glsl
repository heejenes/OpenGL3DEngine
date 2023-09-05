#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{    
    vec4 color = texture(skybox, TexCoords);
    float exponent = 1.4f;
    FragColor = vec4(
        pow(color.x, exponent),
        pow(color.y, exponent),
        pow(color.z, exponent),
        pow(color.w, exponent)
    );
}