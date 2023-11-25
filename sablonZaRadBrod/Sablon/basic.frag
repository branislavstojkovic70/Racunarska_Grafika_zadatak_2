#version 330 core
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D uTexture; 
uniform vec3 uColor; 
uniform bool useTexture;

void main() {
    if (useTexture) {
        FragColor = texture(uTexture, TexCoords);
    } else {
        FragColor = vec4(uColor, 1.0); 
    }
}
