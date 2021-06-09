#version 330 core

in vec2 TexCoords;
in vec3 fN;
in vec3 fL;
in float visibility;

uniform sampler2D Texture0;
uniform vec4 ambientLight;
uniform bool TextureSet;



void main() {
    vec3 N = normalize(fN);
    vec3 L = normalize(fL);
    float diffuse_intensity = max(dot(N,L),0.0f);
    if(TextureSet)
    {
        gl_FragColor = 0.3f*ambientLight+(0.7f*diffuse_intensity*texture(Texture0, TexCoords));
    }
    else
    {
        gl_FragColor = 0.35f*vec4(diffuse_intensity,diffuse_intensity,diffuse_intensity,1.0f) + (0.65f*ambientLight);
    }
    gl_FragColor = mix(vec4(0.5f,0.5f,0.5f,1.0f),gl_FragColor,visibility);
}

