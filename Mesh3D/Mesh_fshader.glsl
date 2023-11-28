#version 330 core
uniform float time;
uniform sampler2D head;
uniform sampler2D body;
uniform sampler2D leg;


out vec4 color;
in vec3 fpoint;
in vec3 fnormal_cam;


vec3 O = vec3(0.82,0.68,0.21);


void main() { 
   //Height levels of Armadillo
   float neck_level = 1.0;
   float hip_level =  0.25;
   
   ///TODO1: Spherical mapping: find out u and v
   
   float u = _______________;
   float v =________________;  
   
   
   ///TODO2: Heightwise texturing and blending of textures
   if (fpoint.y > neck_level){   
        color = _____________________;   
   } else if((fpoint.y > hip_level) && (fpoint.y < neck_level)){
        color = _____________________;
            
   } else if(fpoint.y < hip_level){
        color = ____________________;  
   }

   
    
    /// Lambertian/Diffuse shading
    vec3 N = normalize(fnormal_cam);
    vec3 L = normalize(vec3(0,100,-100)); 
    float lamb = dot(N,L);
    color = color * max(lamb, 0);


    /// Specular shading
    if(lamb>0){
        vec3 V = normalize(vec3(0,100,100)); 
        vec3 R = reflect(V,N);
        float glossy = pow( max(dot(-R,L),0), 100);
        color += vec4(vec3(.75)*glossy,0);
    }

    
    
}
