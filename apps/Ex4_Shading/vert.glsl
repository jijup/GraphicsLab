#version 330 core

uniform mat4 model;
uniform mat4 model_rotation;
uniform mat4 MVP;

// Our vposition values are now actual model space positions, and will need to
// be transformed into clip space using the MVP matrix
in vec3 vposition;

// We also take in the model space vertex normals that we will use for lighting
in vec3 vnormal;

out vec3 fposition;
out vec3 fnormal;


void main() {
    // After multiplying with a perspective transformation, the w coordinate
    // will no longer be 1, and OpenGL will use it later to correct for
    // perspective after interpolation
    gl_Position = MVP * vec4(vposition, 1);

    // fposition will allow us to know the world space position of each fragment
    // later on for lighting
    fposition = (model * vec4(vposition, 1)).xyz;

    // Here we apply the model rotation to the normals
    fnormal = (model_rotation * vec4(vnormal, 1)).xyz;
}