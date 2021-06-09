#version 330 core

in vec2 TexCoords;
in vec3 fN;
in vec3 fL;

in mat4 itMV;
in mat4 mod;
in mat4 vie;
in mat4 proj;

uniform sampler2D Texture0;
uniform float fogstart;
uniform float fogend;
uniform vec4 ambientLight;

uniform vec4 LightPos1;
uniform vec4 LightPos2;
uniform vec4 LightPos3;
uniform vec4 LightPos4;

uniform vec4 LightColor1;
uniform vec4 LightColor2;
uniform vec4 LightColor3;
uniform vec4 LightColor4;

#define PI 3.1415926538

vec3 ObjToViewPos(vec3 pos)
{
    return (mod*vie*vec4(pos, 1.0)).xyz;
}
float saturate(float val)
{
    return clamp(val,0.f,1.f);
}
mat3 M4toM3(mat4 matrix)
{
    return mat3(matrix[0].xyz, matrix[1].xyz, matrix[2].xyz);
}

vec3 ShadeVertexLightsFull (vec4 vertex, vec3 normal, int lightCount, int spotLight)
{
    vec4 lightpos[4] = {LightPos1,LightPos2,LightPos3,LightPos4};
    vec4 lightcol[4] = {LightColor1,LightColor2,LightColor3,LightColor4};
    vec3 viewpos = ObjToViewPos(vertex.xyz);
    mat3 itMV3 = M4toM3(itMV);
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

void main() {
    vec3 N = normalize(fN);
    vec3 L = normalize(fL);
    float diffuse_intensity = max(dot(N,L),0.0f);

    gl_FragColor = diffuse_intensity*texture(Texture0, TexCoords);
    //gl_FragColor = vec4(TexCoords,0.1f,1.0f);
    //gl_FragColor = vec4(diffuse_intensity,diffuse_intensity,diffuse_intensity,0.5f);
}

