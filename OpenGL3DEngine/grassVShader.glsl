
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec4 aColor; 
layout (location = 4) in mat4 instanceMatrix;
layout (location = 8) in float windStrength;
layout (location = 9) in float windFrequency;
layout (location = 10) in float grassColor;
  
out vec3 objectColor; // output a color to the fragment shader
out float opacity;
out vec4 worldPos;
out vec4 localPos;
out vec3 Normal;

uniform float time;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

const float windDirDegrees = 20.f;
const vec2 windDir = vec2(sin(windDirDegrees), cos(windDirDegrees));

void main()
{
    worldPos = instanceMatrix * vec4(aPos, 1.0f);
    float windWorldHeightCo = abs(worldPos.y);
    float windLocalHeightCo = 5 * 0.005f * (aPos.y * aPos.y);
    float windYDisplacement = 0.01f * (-1.f) * (aPos.y * aPos.y);
    vec4 windDisplacement = windLocalHeightCo * windWorldHeightCo * vec4(windDir.x, windYDisplacement, windDir.y, 0.0f) * (0.5f + sin(windFrequency * time + windStrength));
    worldPos = worldPos + windDisplacement;
    localPos = vec4(aPos, 1.0f); // + windDisplacement;

    gl_Position = projection * view * worldPos;
    Normal = (instanceMatrix * vec4(aNormal, 1.0f)).xyz;
    objectColor = aColor.xyz * min(1.0f, max(0.3f, grassColor));
    opacity = aColor.w;
}       