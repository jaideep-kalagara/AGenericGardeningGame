#version 330 core
out vec4 FragColor;

in vec2 vUV;

uniform sampler2D uTex;

void main() {
    vec4 texColor = texture(uTex, vUV);
    FragColor = texColor;  // multiply by tint
}
