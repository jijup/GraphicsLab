#version 330 core

// This is a per-vertex input attribute
in vec3 vposition;

// This is a per-fragment output attribute that we will use later in the
// fragment shader as an input
out vec2 fposition;

// This function runs once for each vertex
//
// Here is where you want to put any logic that acts on individual points
// of your primitives, i.e. applying transformations and projections
void main() {

    // gl_Position is a built in vertex shader output that defines where on the
    // screen the vertex should be drawn.
    //
    // It is a vec4 because perspective-corrected interpolation requires
    // division by w after interpolation occurs, and interpolation is done by
    // OpenGL for you, meaning that you need to give it the w values
    gl_Position = vec4(vposition, 1);

    // This is an example of a user-defined output. We write one value per
    // vertex, and OpenGL will perform interpolation with the values at the
    // other vertices of the primitive to get a value for each pixel the
    // primitive covers
    fposition = vposition.xy;
}