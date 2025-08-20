#version 330 core

layout (location = 0) in vec2 vertPos;

layout (location = 1) in vec2 texCoords;

uniform vec2 uPos;
uniform vec2 uScale;

out vec2 vUV;

void main() {
    gl_Position = vec4(vertPos.x * uScale.x + uPos.x, vertPos.y * uScale.y + uPos.y, 0.0, 1.0);
    vUV = texCoords;
}