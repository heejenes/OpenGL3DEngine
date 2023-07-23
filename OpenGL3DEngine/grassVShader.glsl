
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec4 aColor; 
layout (location = 4) in mat4 instanceMatrix;
layout (location = 8) in float windStrength;
layout (location = 9) in float windFrequency;
layout (location = 10) in float grassColor;
layout (location = 11) in vec3 translation;
layout (location = 12) in vec3 scale;
  
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

const float curveDirDegrees = 0.f;
const vec2 curveDir = vec2(sin(curveDirDegrees), cos(curveDirDegrees));

const float sinWaveOffset = 0.9f;

vec4 GetDownVec() {
    vec3 temp = aPos - vec3(0, 0.3f, 0);
    float curveLocalHeightCo = 5 * 0.02f * (temp.y * temp.y);
    float curveYDisplacement = 0.05f * (-1.f) * (temp.y * temp.y);
    vec4 curveDisplacement = curveLocalHeightCo * vec4(curveDir.x, curveYDisplacement, curveDir.y, 0.0f);

    vec4 tempWorldPos = instanceMatrix * (vec4(temp, 1.0f) + curveDisplacement);
    tempWorldPos = vec4(tempWorldPos.x * scale.x, tempWorldPos.y * scale.y, tempWorldPos.z * scale.z, 1.0f);

    float windIntensity = 0.7f * (sinWaveOffset + sin(windFrequency * time + windStrength));

    float windGrassDirectionDot = 1.0f + 0.1f * abs(dot(tempWorldPos.xz, windDir));
    float windWorldHeightCo = 1.0f + abs(translation.y);
    float windLocalHeightCo = 5.0f * 0.015f * (temp.y * temp.y);
    float windYDisplacement = (-0.3f) * windWorldHeightCo * windLocalHeightCo * abs(windIntensity);
    vec4 windDisplacement = windGrassDirectionDot * windLocalHeightCo * windWorldHeightCo * vec4(windDir.x, windYDisplacement, windDir.y, 0.0f) * windIntensity;
    
    return tempWorldPos + windDisplacement;
}

void main()
{
    float curveLocalHeightCo = 5 * 0.02f * (aPos.y * aPos.y);
    float curveYDisplacement = 0.05f * (-1.f) * (aPos.y * aPos.y);
    vec4 curveDisplacement = curveLocalHeightCo * vec4(curveDir.x, curveYDisplacement, curveDir.y, 0.0f);

    worldPos = instanceMatrix * (vec4(aPos, 1.0f) + curveDisplacement);
    worldPos = vec4(worldPos.x * scale.x, worldPos.y * scale.y, worldPos.z * scale.z, 1.0f);

    float windIntensity = 0.7f * (sinWaveOffset + sin(windFrequency * time + windStrength));

    float windGrassDirectionDot = 1.0f + 0.1f * abs(dot(worldPos.xz, windDir));
    float windWorldHeightCo = 1.0f + abs(translation.y);
    float windLocalHeightCo = 5.0f * 0.015f * (aPos.y * aPos.y);
    float windYDisplacement = (-0.3f) * windWorldHeightCo * windLocalHeightCo * abs(windIntensity);
    vec4 windDisplacement = windGrassDirectionDot * windLocalHeightCo * windWorldHeightCo * vec4(windDir.x, windYDisplacement, windDir.y, 0.0f) * windIntensity;
    

    worldPos = worldPos + windDisplacement + vec4(translation, 0.0f);
    localPos = vec4(aPos, 1.0f);

    gl_Position = projection * view * worldPos;

    vec3 norm = (instanceMatrix * vec4(aNormal, 1.0f)).xyz;
    vec3 tempLocal = worldPos.xyz - translation;
    vec3 sideVec = cross(norm, vec3(0, 1, 0));
    vec3 downVec = GetDownVec().xyz - tempLocal;
    norm = cross(sideVec, downVec);

    Normal = norm;
    objectColor = aColor.xyz * min(1.0f, max(0.3f, grassColor));
    opacity = aColor.w;
}       