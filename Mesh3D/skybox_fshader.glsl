R"(
#version 330 core
out vec4 FragColor;

in vec3 texCoords;

uniform samplerCube skybox;

void main() {    
    
     ////TODO3(c):
     vec4 skyColor = texture(_____, texCoords); 
     FragColor = skyColor;
}
)"