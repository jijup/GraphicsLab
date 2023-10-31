#version 330 core
in vec3 vpoint;
in vec2 vtexcoord;

out vec2 uv;

//TODO 3: Define Scaling and Rotate functions. GLSL use column major order

mat4 S(float s){
    return mat4(-----);//use mat3() with 's'
}

mat4 R(float degrees){
    mat3 R = mat3(1);
    float alpha = radians(degrees);
    R[0][0] =  ------; //cos
    R[0][1] =  ------; //sin
    R[1][0] = -------; //-sin
    R[1][1] =  ------; //cos
    return mat4(R);
}

mat4 T(float t) {
    mat4 T = mat4(1);
    T[3][0] = t;
    T[3][1] = t;
    return T;
}

uniform float time;


void main() {

     //TODO4: Use the transformation functions to move the quad geometry. Modify the following statement.
	gl_Position = vec4(vpoint, 1.0);	
	uv = vtexcoord; 
}
