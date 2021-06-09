#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec2 TexCoords;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//Light
uniform vec3 vLight; //light pos
out vec3 fN; //light normal
out vec3 fL; /// light vec


out float visibility;
void main(void)
{
    float density = 0.1f;
    float gradient = 1.5f;
    vec4 PosRelToCam = model*view*vec4(position,1.0f);
    fN = normal;
    fL = vLight;
    gl_Position = projection * view * model * vec4(position, 1.0f);
    TexCoords = texCoords;

    float distance = length(PosRelToCam.xyz);
    visibility = exp(-1.f*pow((distance*density),gradient));
}
