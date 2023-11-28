R"(
#version 330 core
in vec3 vposition;
out vec3 texCoords;

uniform mat4 projection;
uniform mat4 view;



void main() {
    // the skybox unfortunately is not infinite. We make it big enough that we never cross it
    vec4 pos = projection * view * vec4(1000*vposition, 1.0f);
    

    // Having z equal w will always result in a depth of 1.0f which cannot be culled because of glDepthFunc(GL_LEQUAL);
    gl_Position = vec4(pos.x, pos.y, pos.w, pos.w);
    
    // We calculate the texCoords based on the position of the vertices
    texCoords =  vec3(vposition.x, -vposition.y, vposition.z);
    
}   
)"