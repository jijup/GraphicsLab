#version 330 core 
uniform float time;
uniform vec3 viewPos;

in vec3 vpoint;
in vec3 vnormal;
out vec3 fpoint; 

/// helper functions
mat4 T(float x, float y, float z){ 
    mat4 _T = mat4(1);  
    _T[3] = vec4(x,y,z,1);
    return _T;
}

mat4 S(float s) {
    return mat4(mat3(s));
}


mat4 lookat(vec3 origin, vec3 at, vec3 up){
    mat4 M = mat4(1);
    vec3 z = normalize(at-origin);
    vec3 x = normalize(cross(z, up));
    vec3 y = normalize(cross(x, z));
    M[0] = vec4(x,0);
    M[1] = vec4(y,0);
    M[2] = vec4(z,0);
    M[3] = vec4(origin,1);
    return inverse(M);
}

mat4 ortho(float l, float r, float b, float t, float n, float f){
    mat4 T = mat4(1);
    T[3] = vec4(-(r+l)/2, -(t+b)/2, -(f+n)/2, 1);
    mat4 S = mat4(1);
    S[0][0] = 2.0/(r-l);
    S[1][1] = 2.0/(t-b);
    S[2][2] = 2.0/(f-n);
    return S*T;
}

mat4 persp(float n, float f){
    mat4 P = mat4(1); //!!! column assembly
    P[0][0] = n;
    P[1][1] = n;
    P[2] = vec4(0, 0, (n+f), -n*f);
    P[3] = vec4(0, 0, 1, 0);
    return transpose(P);
}

mat4 persp_fov(float theta_deg, float width_over_height, float near, float far){
    float right = near * tan(.5* radians(theta_deg) );
    float top = right / width_over_height;
    float bottom = -top;
    float left = -right;
    
    mat4 _ortho = ortho(left,right,bottom,top,near,far);
    mat4 _persp = persp(near, far);
    return _ortho*_persp;    
}

out vec3 fnormal_cam;



///-- DEFAULT MATRICES
mat4 MODEL = mat4(1);
mat4 VIEW = mat4(1);
mat4 PROJ = mat4(1);


void main() {
    
   /// view matrix     
    VIEW = lookat( 2*vec3(cos(time), 0, sin(time)), vec3(0,0,0), vec3(0,1,0) );     
    //PROJ = persp_fov(120, 1, 0.005, 5); //perspective projection
    PROJ = ortho(-5, 5, -5, 5, -5, 5); //orthographic projection
    fnormal_cam = inverse(transpose(mat3(VIEW*MODEL))) * vnormal;
    
    fpoint = vpoint;
    gl_Position = PROJ * VIEW * MODEL * S(1.5)*vec4(vpoint, 1.0);
    
}
