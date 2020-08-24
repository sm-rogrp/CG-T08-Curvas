#version 430

out vec4 color;

uniform vec3 u_color;

in vec4 fcolor;

void main(void) {
    color = fcolor;
}