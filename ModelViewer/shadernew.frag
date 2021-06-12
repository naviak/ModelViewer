#version 330 core

noperspective in vec2 TexCoords;
in vec3 fN;
in vec3 fL;
in float visibility;

uniform sampler2D Texture0;
uniform vec4 ambientLight;
uniform bool TextureSet;

uniform float rt_h;
uniform float rt_w;


vec4 posterization(vec4 color)
{
    vec3 c = color.rgb;
    c = pow(c, vec3(0.6f, 0.6f, 0.6f));
    c = c * 16.f;
    c = floor(c);
    c = c / 16.f;
    c = pow(c, vec3(1.0/0.6f));
    return vec4(c,1.0f);
}

vec4 pixelation(sampler2D sceneTex, vec2 uv) //uv - texture coords
{
    float dx = 15.f*(1.f/rt_w);
    float dy = 10.f*(1.f/rt_h);
    vec2 coord = vec2(dx*floor(uv.x/dx), dy*floor(uv.y/dy));
    vec4 tc = texture(sceneTex, coord);
    //return vec4(1.0f);
    return tc;
}

void main() {
    vec3 N = normalize(fN);
    vec3 L = normalize(fL);
    float diffuse_intensity = max(dot(N,L),0.0f);
    if(TextureSet)
    {
        gl_FragColor = 0.3f*ambientLight+(0.7f*diffuse_intensity*pixelation(Texture0, TexCoords));
    }
    else
    {
        gl_FragColor = 0.35f*vec4(diffuse_intensity,diffuse_intensity,diffuse_intensity,1.0f) + (0.65f*ambientLight);
    }
    vec4 c = mix(vec4(0.5f,0.5f,0.5f,1.0f),gl_FragColor,visibility);

    gl_FragColor = posterization(c);

}

