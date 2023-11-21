R"(
#version 330 core

uniform sampler2D grass;
uniform sampler2D rock;
uniform sampler2D sand;
uniform sampler2D snow;

uniform vec3 viewPos;
uniform float waterHeight;
uniform vec3 skyColor;
uniform vec3 lightPos;

in vec2 uv;
in vec3 fragPos;
in float height;
in vec3 normal;
in float slope; 
in vec3 distanceFromCamera;

out vec4 color;


void main() {

    
   
    vec4 pureSandCol = texture(sand, uv);
    vec4 col = pureSandCol;
     

    // Blinn-Phong constants are placed here so we can update them at each level
    float ka = 0.2f, kd = 0.3f, ks = 0.7f, p = 0.8f;    
    
    ///TODO4: Blinn-Phong shading. Compute diffuse, specular and ambient components and add.
    // Blinn-Phong calculation
    vec3 lightDir = _____________;
    float diffuse = _____________;

    vec3 viewDirection = ____________;
    vec3 halfVector = __________;
    float specular = ____________;

    // You may use white as the ambient color .
    col = ka*vec4(1.0f, 1.0f, 1.0f, 1.0f) + diffuse*col + specular*col;

    color = vec4(col); //Comment this and uncomment the below code to render fog

    // visibility calculation. 
    //add fog so that we can hide 'render distance'  
    // we mix the color with the skycolor based on the distance from the camera
    // we use a visibility formula to detect how far an object is from the camera
    // 1 => render normaly, 0 => fade into the skycolor
    // visibility takes the distance from the camera and use an exponential decrease so that the fog scaling looks more natural
    // visibility = exp(-pow(distance * density, gradient)) is such a formula
    // density = thickness of the fog, higher value means less visiblr
    // gradient = how quickly the visibility decreases with distance
    
    /*float density = 0.1;
    float gradient = 1.5;
    float distance = length(distanceFromCamera.xyz);
    float visibility = exp(-pow(distance * density, gradient));
    visibility = clamp(visibility, 0.0, 1.0);
    color = mix(vec4(0.6f, 0.8f, 0.9f, 1.0f),  col, visibility);*/
    
}
)"