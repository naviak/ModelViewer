#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

noperspective out vec2 TexCoords;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//Light
uniform vec3 vLight; //light pos
out vec3 fN; //light normal
out vec3 fL; /// light vec

out float visibility;

vec4 ObjectToViewPos(vec3 pos)
{
    return view * model * vec4(pos, 1.0f);
}

vec4 ObjectToClipPos(vec3 pos)
{
    return projection * view * model * vec4(pos, 1.0f);
}

void main(void)
{
    float density = 0.1f;
    float gradient = 1.5f;
    vec4 PosRelToCam = ObjectToViewPos(position);
    fN = normal;
    fL = vLight;

    //Pixel Snapping

    //gl_Position = projection * view * model * vec4(position, 1.0f);
    vec4 snapToPixel = projection * view * model * vec4(position, 1.0f);
	vec4 vertex = snapToPixel;
    vertex.xyz = snapToPixel.xyz/snapToPixel.w;
    vertex.x = floor(160 * vertex.x) / 160;
	vertex.y = floor(120 * vertex.y) / 120;
	vertex.xyz *= snapToPixel.w;
	gl_Position = vertex;

    //Affine Texturing
    float distance = length(PosRelToCam.xyz);
    //float affine = distance + ((position.w * 8.0) / distance) * 0.5;

    TexCoords = texCoords;

    visibility = exp(-1.f*pow((distance*density),gradient));
}
