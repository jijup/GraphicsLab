R"(
#version 330 core

in vec3 vposition;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform vec3 viewPos; 

// clip plane
uniform vec3 clipPlaneNormal;
uniform float clipPlaneHeight;

out vec2 uv;
out vec3 fragPos;
out vec3 normal;
out float height;
out float slope;
out vec3 distanceFromCamera;


//Perlin noise on GPU
float Perlin2D( vec2 P ) {
    //  https://github.com/BrianSharpe/Wombat/blob/master/Perlin2D.glsl

    // establish our grid cell and unit position
    vec2 Pi = floor(P);
    vec4 Pf_Pfmin1 = P.xyxy - vec4( Pi, Pi + 1.0 );

    // calculate the hash
    vec4 Pt = vec4( Pi.xy, Pi.xy + 1.0 );
    Pt = Pt - floor(Pt * ( 1.0 / 71.0 )) * 71.0;
    Pt += vec2( 26.0, 161.0 ).xyxy;
    Pt *= Pt;
    Pt = Pt.xzxz * Pt.yyww;
    vec4 hash_x = fract( Pt * ( 1.0 / 951.135664 ) );
    vec4 hash_y = fract( Pt * ( 1.0 / 642.949883 ) );

    // calculate the gradient results
    vec4 grad_x = hash_x - 0.49999;
    vec4 grad_y = hash_y - 0.49999;
    vec4 grad_results = inversesqrt( grad_x * grad_x + grad_y * grad_y ) * ( grad_x * Pf_Pfmin1.xzxz + grad_y * Pf_Pfmin1.yyww );

    // Classic Perlin Interpolation
    grad_results *= 1.4142135623730950488016887242097;  // scale things to a strict -1.0->1.0 range  *= 1.0/sqrt(0.5)
    vec2 blend = Pf_Pfmin1.xy * Pf_Pfmin1.xy * Pf_Pfmin1.xy * (Pf_Pfmin1.xy * (Pf_Pfmin1.xy * 6.0 - 15.0) + 10.0);
    vec4 blend2 = vec4( blend, vec2( 1.0 - blend ) );
    return dot( grad_results, blend2.zxzx * blend2.wwyy );
}



void main() {

    // displace the position so that we get an infinite world
	vec3 position = vposition + (vec3(viewPos.x, viewPos.y, 0)); 

    // we texture based on the perturbed position now so that the texturing scale with the infinite world
    // we get the tex coordinate from the perturbed position normalised in [0, 1], so we bring to 0 to f_width 
    // and we divide by f_width
    uv = (position.xy + 20/2) / 20; 

    // we tile the textures to improve the resolution => instead of [0, 1], we use [0, 40]
    // openGL with GL_REPEAT will just mod it for us
    int num_tiles = 40; 
    uv = (uv * num_tiles); // make opengl repeat the texture so we get higher resolution 
    
    ///TODO2: Displacement of grid vertex using the height values obtained from Perlin2D function
    // Calculate height. 
    float h = ________;
    height = h;
    // give new disturbed position
    fragPos = _________;
    gl_Position = P*V*M*vec4(fragPos, 1.0f);

    ///TODO3: Calculate the vertex normals using the adjacent vertices

    // find the normal by finding two vectors on the plane next to the point and cross producting them
    vec3 A = ____________________;
    vec3 B = ____________________;
    vec3 C = ____________________;
    vec3 D = ____________________;
    vec3 n = normalize( cross(A - B , C - D) );
    normal = n;

    // the up vector is (0, 0, 1) so the dot of the top and the normal vector is normal.z
    // we take acos to get the actual gradient
    slope = acos(normal.z);

    

    // set distance from camera to scale the visibility
    distanceFromCamera = fragPos - viewPos;


    // we also add a clipping plane for the water rendering...
    // set up clipping planes
    // every vertex now needs to give a distance from it to the clip plane
    // if that distance is positive, the vertex is rendered
    // if that distance is negative, the vertex is not rendered
    // opengl will interpolate the distance for points in the middle of vertices to clip them
    
    // to define a plane, we can just use the plane equations
    // we can declare a z = k plane with a normal of (0, 0, -1) and a height
    vec4 plane = vec4(clipPlaneNormal, clipPlaneHeight); 
    
    // to find the distance from a vertex to a plane, you just need to 
    // do the dot product of the vertex with the plane vector
    // need to compare with NON view-space coordinates, i.e, pre-transformation as plane is defined pre-transformation
    
    float distance_of_this_vertex = dot(plane, vec4(fragPos, 1.0));      
    gl_ClipDistance[0] = distance_of_this_vertex;
}
)"