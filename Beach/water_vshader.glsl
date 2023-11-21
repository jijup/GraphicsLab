R"(
#version 330 core

in vec3 vposition;
in vec2 vtexcoord;
out vec4 clipSpaceCoordinates;
out vec2 uv;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform vec3 viewPos; 


void main() {
    //Set gl_Position
    vec4 position = P*V*M*vec4(vposition +(vec3(viewPos.x, viewPos.y, 0)), 1.0f);    

    clipSpaceCoordinates = position;
    gl_Position = position;
    uv = vtexcoord;
    
}
)"