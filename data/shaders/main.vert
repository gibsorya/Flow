#version 410 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out vec3 outColor;

uniform mat4 vp;
uniform mat4 model;

void main() {
    gl_Position = vp * model * vec4(position, 1.0);
    outColor = color;
}