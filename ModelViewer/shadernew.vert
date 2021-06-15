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

uniform vec4 LightPos1;
uniform vec4 LightPos2;
uniform vec4 LightPos3;
uniform vec4 LightPos4;

uniform vec4 LightColor1;
uniform vec4 LightColor2;
uniform vec4 LightColor3;
uniform vec4 LightColor4;

uniform vec4 ambLight;
out vec3 dfl;
out float visibility;
#define PI 3.141516f
vec4 ObjectToViewPos(vec3 pos)
{
    return view * model * vec4(pos, 1.0f);
}

vec4 ObjectToClipPos(vec3 pos)
{
    return projection * view * model * vec4(pos, 1.0f);
}

mat3 M4toM3(mat4 matrix)
{
    return mat3(matrix[0].xyz, matrix[1].xyz, matrix[2].xyz);
}

float saturate(float val)
{
    return clamp(val,0.f,1.f);
}

mat4 itMV()
{
    return inverse(transpose(model*view));
}

vec3 ShadeVertexLightsFull (vec3 vertex, vec3 normal, int lightCount, int spotLight)
{
    vec4 lightpos[4] = {LightPos1,LightPos2,LightPos3,LightPos4};
    vec4 lightcol[4] = {LightColor1,LightColor2,LightColor3,LightColor4};
    vec3 viewpos = ObjectToViewPos(vertex).xyz;
    mat3 itMV3 = M4toM3(itMV());
    vec3 viewN = normalize(itMV3 * normal);
    float spotAngle = PI;
    vec4 LightAtten = vec4(cos(spotAngle/2.f),1/cos(spotAngle/4.f),0.001f,1000.f);

    vec3 lightColor = 0.3f*ambLight.xyz;
    for (int i = 0; i < lightCount; i++) {
        vec3 toLight = lightpos[i].xyz - viewpos.xyz * lightpos[i].w;
        float lengthSq = dot(toLight, toLight);

        // don't produce NaNs if some vertex position overlaps with the light
        lengthSq = max(lengthSq, 0.0001f);

        toLight = normalize(toLight);

        float atten = 1.0 / (1.0 + lengthSq * LightAtten.z);
        if (spotLight)
        {
            float rho = max(0, dot(toLight, lightpos[i].xyz));
            float spotAtt = (rho - LightAtten.x) * LightAtten.y;
            atten *= saturate(spotAtt);
        }

        float diff = max(0.f, dot(viewN, toLight));
        lightColor += lightcol[i].rgb * (diff * atten);
    }
    return lightColor;
    //return vec3(1.f);
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
    vec4 snapToPixel = ObjectToClipPos(position);
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
    dfl = ShadeVertexLightsFull(position,normal,1,1);
}
