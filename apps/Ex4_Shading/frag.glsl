#version 330 core

// World space position of the camera: we can use this to get the view direction
uniform vec3 eye;

// Parameters of the lighting
uniform vec3 light_dir;
uniform vec3 light_color;
uniform vec3 ambient_light;

// Parameters of the material
uniform vec3 bunny_color;
uniform vec3 bunny_specular;
uniform float specular_power;

in vec3 fposition;
in vec3 fnormal;

out vec4 fcolor;


void main() {
    // Remember that linear interpolation will not result in normalized vectors
    vec3 normal = normalize(fnormal);

    // The view direction is the direction from the camera to the fragment
    vec3 view = normalize(eye - fposition);

    // This is the contribution of diffuse reflection to the total
    //
    // Diffuse reflection depends on the angle between the surface and the light
    // direction. The dot product will go to zero when the light becomes
    // parallel to the surface 
    float d_weight = max(-dot(light_dir, normal), 0);

    // This is the contribution of specular reflection
    //
    // Specular reflection is light that is reflected in a mirror-like way and
    // can be computed by finding the deviation between the view direction and
    // the direction of reflected light
    float s_weight = pow(max(dot(reflect(view, normal), light_dir), 0),
                         specular_power);

    // Adding everything up gives us the Phong reflection equation
    //
    // In this case, the light color and material color are both vectors, and
    // the multiplication of vectors in glsl is treated as a component-wise
    // operation, so the result is essentially a seperate lighting computation
    // for each RGB channel
    vec3 illumination = ambient_light * bunny_color
                      + d_weight * light_color * bunny_color
                      + s_weight * light_color * bunny_specular;

    // Combine illumination with alpha value of 1 to make an RGBA color
    fcolor = vec4(illumination, 1);
}