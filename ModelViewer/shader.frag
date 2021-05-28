in vec2 TexCoords;

uniform sampler2D Texture0;

void main() {
    gl_FragColor = texture(Texture0, TexCoords);
}
