R"(
#version 330 core
uniform sampler2D noiseTex;

in vec3 vposition;
in vec2 vtexcoord;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec2 uv;
out vec3 fragPos;

out float waterHeight;

void main() {

    uv = vtexcoord;

    // Earth scene
    float water = 0.7f;

    
    // TODO: Calculate height
    

    // TODO: Set fragment position
    //fragPos = ?

    // Set gl_Position
    gl_Position = P*V*M*vec4(fragPos, 1.0f);

    // Set height of water
    waterHeight = water;
}
)"
