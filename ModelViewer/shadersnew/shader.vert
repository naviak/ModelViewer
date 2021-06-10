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

out vec3 lightning; //light

uniform vec4 LightPos1;
uniform vec4 LightPos2;
uniform vec4 LightPos3;
uniform vec4 LightPos4;

uniform vec4 LightColor1;
uniform vec4 LightColor2;
uniform vec4 LightColor3;
uniform vec4 LightColor4;

vec3 ObjToViewPos(vec3 pos)
{
    return (model*view*vec4(position, 1.0)).xyz;
}
float saturate(float val)
{
    return clamp(val,0.f,1.f);
}
mat3 M4toM3(mat4 matrix)
{
    return mat3(matrix[0].xyz, matrix[1].xyz, matrix[2].xyz);
}

vec3 ObjectToClipPos(vec3 pos)
{
    return (model * view * projection * vec4(pos,1.0f)).xyz;
}

mat4 itMV()
{
    return inverse(transpose((model*view));
}

vec3 ShadeVertexLightsFull (vec4 vertex, vec3 normal, int lightCount, int spotLight)
{
    vec4 lightpos[4] = {LightPos1,LightPos2,LightPos3,LightPos4};
    vec4 lightcol[4] = {LightColor1,LightColor2,LightColor3,LightColor4};
    vec3 viewpos = ObjToViewPos(vertex.xyz);
    mat3 itMV3 = M4toM3(itMV());
    vec3 viewN = normalize(itMV3 * normal);
    float spotAngle = PI;
    vec4 LightAtten = vec4(cos(spotAngle/2),1/cos(spotAngle/4),0.0f,1000.f);

    vec3 lightColor = ambientLight.xyz;
    for (int i = 0; i < lightCount; i++) {
        vec3 toLight = lightpos[i].xyz - viewpos.xyz * lightpos[i].w;
        float lengthSq = dot(toLight, toLight);

        // don't produce NaNs if some vertex position overlaps with the light
        lengthSq = max(lengthSq, 0.0001f);

        toLight *= 1/sqrt(lengthSq);

        float atten = 1.0 / (1.0 + lengthSq * LightAtten.z);
        if (spotLight)
        {
            float rho = max (0, dot(toLight, lightpos[i].xyz));
            float spotAtt = (rho - LightAtten.x) * LightAtten.y;
            atten *= saturate(spotAtt);
        }

        float diff = max (0, dot(viewN, toLight));
        lightColor += lightcol[i].rgb * (diff * atten);
    }
    return lightColor;
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
}
