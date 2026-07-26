#version 410 core

layout (location = 0) in vec3 positions;
layout (location = 2) in vec3 colors;

out vec3 outColor;

void main() {
    gl_Position = vec4(positions, 1.0);
    outColor = colors;
}