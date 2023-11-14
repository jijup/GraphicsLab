R"(
#version 330 core

// Uniforms
uniform sampler2D noiseTex;
uniform sampler2D grass;
uniform sampler2D rock;
uniform sampler2D sand;
uniform sampler2D snow;
uniform sampler2D water;
uniform sampler2D lunar;

uniform float waveMotion;
uniform vec3 viewPos;

// In
in vec2 uv;
in vec3 fragPos;
in float waterHeight;

// Out
out vec4 color;

void main() {

    // Directional light source
    vec3 lightDir = normalize(vec3(1,1,1));

    // Texture size in pixels
    ivec2 size = textureSize(noiseTex, 0);

    /// TODO: Calculate surface normal N
    /// HINT: Use textureOffset(,,) to read height at uv + pixelwise offset
    /// HINT: Account for texture x,y dimensions in world space coordinates (default f_width=f_height=5)
    vec3 A = vec3( 0 );
    vec3 B = vec3( 0 );
    vec3 C = vec3( 0 );
    vec3 D = vec3( 0 );
    vec3 N = normalize( cross(normalize(A-B), normalize(C-D)) );

    /// TODO: Texture according to height and slope
    /// HINT: Read noiseTex for height at uv

    /// TODO: Calculate ambient, diffuse, and specular lighting
    /// HINT: max(,) dot(,) reflect(,) normalize()

    color = vec4(0,0,0,1);
}
)"
