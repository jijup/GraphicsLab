#version 330 core

uniform mat4 model;

in vec3 custom_vert_attrib;
out vec2 uv;

void main() {
    // Apply the model transformation matrix
    gl_Position = model * vec4(custom_vert_attrib, 1);

    // Map from clip space [-1, 1] to texture space [0, 1]
    uv = vec2(custom_vert_attrib.x + 0.5, custom_vert_attrib.y + 0.5);
}