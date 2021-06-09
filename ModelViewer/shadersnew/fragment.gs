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



vec3 ObjToViewPos(vec3 pos)
{
    return mul(mod*vie, float4(pos, 1.0)).xyz;
}

mat3 M4toM3(mat4 matrix)
{
    return mat3(matrix[0].xyz, matrix[1].xyz, matrix[2].xyz);
}

vec3 ShadeVertexLightsFull (vec4 vertex, vec3 normal, int lightCount, int spotLight)
{
    vec3 viewpos = ObjToViewPos(vertex.xyz);
    mat3 itMV3 = M4toM3(itMV);
    float3 viewN = normalize(mul(itMV3, normal));

    float3 lightColor = UNITY_LIGHTMODEL_AMBIENT.xyz;
    for (int i = 0; i < lightCount; i++) {
        float3 toLight = unity_LightPosition[i].xyz - viewpos.xyz * unity_LightPosition[i].w;
        float lengthSq = dot(toLight, toLight);

        // don't produce NaNs if some vertex position overlaps with the light
        lengthSq = max(lengthSq, 0.000001);

        toLight *= rsqrt(lengthSq);

        float atten = 1.0 / (1.0 + lengthSq * unity_LightAtten[i].z);
        if (spotLight)
        {
            float rho = max (0, dot(toLight, unity_SpotDirection[i].xyz));
            float spotAtt = (rho - unity_LightAtten[i].x) * unity_LightAtten[i].y;
            atten *= saturate(spotAtt);
        }

        float diff = max (0, dot (viewN, toLight));
        lightColor += unity_LightColor[i].rgb * (diff * atten);
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

