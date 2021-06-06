#version 330 core

in vec2 TexCoords;
in vec3 fN;
in vec3 fL;

uniform sampler2D Texture0;

void main() {
    vec3 N = normalize(fN);
    vec3 L = normalize(fL);
    float diffuse_intensity = max(dot(N,L),0.0f);

    gl_FragColor = diffuse_intensity*texture(Texture0, TexCoords);
    //gl_FragColor = vec4(TexCoords,0.1f,1.0f);
    //gl_FragColor = vec4(diffuse_intensity,diffuse_intensity,diffuse_intensity,0.5f);
}

