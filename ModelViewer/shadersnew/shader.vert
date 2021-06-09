#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec2 TexCoords;

out mat4 itMV;
out mat4 mod;
out mat4 vie;
out mat4 proj;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//Light
uniform vec3 vLight; //light pos
out vec3 fN; //light normal
out vec3 fL; /// light vec

vec3 ObjectToClipPos(vec3 pos)
{
    return (model * view * projection * vec4(pos,1.0f)).xyz;
}

void main(void)
{
    //Vertex snapping
    vec3 snapToPixel = ObjectToClipPos(position);
	vec4 vertex = vec4(snapToPixel,1.0f);
	vertex.xyz = snapToPixel.xyz / snapToPixel.w;
	vertex.x = floor(160 * vertex.x) / 160;
	vertex.y = floor(120 * vertex.y) / 120;
	vertex.xyz *= snapToPixel.w;
	gl_Position = vertex;

    
    fN = normal;
    fL = vLight;
    gl_Position = projection * view * model * vec4(position, 1.0f);
    TexCoords = texCoords;
    itMV = inverse(transpose((model*view));
    mod = model;
    vie = view;
    proj = projection;
}
