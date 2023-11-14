#version 330 core

// This is a uniform input, which is set before the draw call and has the same
// value for every invocation of the shader. Note that uniforms can be accessed
// from any shader stage (vertex, fragment, geometry) where they are declared,
// and can be accessed from multiple shader stages at the same time
uniform float time;

// This is the value of the output that we wrote to in the vertex shader, which
// has been interpolated by OpenGL
in vec2 fposition;

// This is the output of our draw operation (usually a color for a single pixel)
out vec4 fcolor;

// This runs once per fragment (fragment == pixel usually if not doing MSAA)
//
// Here is typically where shading logic goes (unless you are doing Gouraud,
// then it would go in the vertex shader). Also, fancy processing effects that
// apply filters to an image (blurs, edge highlighting, etc.) usually put logic
// here
void main() {
    fcolor = vec4(0.5 + sin(8 * fposition.x + time / 9) / 2,
                  0.5 + cos(11 * fposition.y + time / 16) / 2,
                  0,
                  1);
}