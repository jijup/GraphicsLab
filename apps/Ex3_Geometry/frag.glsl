#version 330 core

in vec2 uv;
out vec4 fcolor;

void main() {
    fcolor = vec4(uv.x, uv.y, 0, 1);
}