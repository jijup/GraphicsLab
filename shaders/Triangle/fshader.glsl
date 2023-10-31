#version 330 core
// shaders/Triangle//fshader.glsl
in vec4 vertexColor;
out vec4 color;
void main(){
    color = vertexColor;
}
