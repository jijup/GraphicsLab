#include <OpenGP/GL/Application.h>
#include "OpenGP/GL/Eigen.h"

#include "loadTexture.h"
#include "noise.h"

using namespace OpenGP;
const int width=1280, height=720;

const char* skybox_vshader =
#include "skybox_vshader.glsl"
;
const char* skybox_fshader =
#include "skybox_fshader.glsl"
;

const char* terrain_vshader =
#include "terrain_vshader.glsl"
;
const char* terrain_fshader =
#include "terrain_fshader.glsl"
;

const unsigned resPrim = 999999;
constexpr float PI = 3.14159265359f;

void init();
void genTerrainMesh();
void genCubeMesh();
void drawSkybox();
void drawTerrain();

std::unique_ptr<Shader> skyboxShader;
std::unique_ptr<GPUMesh> skyboxMesh;
GLuint skyboxTexture;

std::unique_ptr<Shader> terrainShader;
std::unique_ptr<GPUMesh> terrainMesh;
std::unique_ptr<R32FTexture> heightTexture;
std::map<std::string, std::unique_ptr<RGBA8Texture>> terrainTextures;

Vec3 cameraPos;
Vec3 cameraFront;
Vec3 cameraUp;

float fov;
float speed;
float speedIncrement;
float halflife;

float yaw;
float pitch;

float waveMotion;

int main(int, char**){

    Application app;

    init();
    genCubeMesh();
    genTerrainMesh();

    // Initialize camera position and direction
    cameraPos = Vec3(0.0f, 0.0f, 3.0f);
    cameraFront = Vec3(0.0f, -1.0f, 0.0f);
    cameraUp = Vec3(0.0f, 0.0f, 1.0f);

    // Initialize FOV and camera speed
    fov = 80.0f;
    speed = 0.01f;
    speedIncrement = 0.002f;

    // Initialize yaw (left/right) and pitch (up/down) angles
    yaw = 0.0f;
    pitch = 0.0f;

    // Initialize motion of waves
    waveMotion = 0.0f;

    // Display callback
    Window& window = app.create_window([&](Window&){

        // Mac OSX Configuration (2:1 pixel density)
        glViewport(0,0,width*2,height*2);

        // Windows Configuration (1:1 pixel density)
        //glViewport(0,0,width,height);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        drawSkybox();
        glClear(GL_DEPTH_BUFFER_BIT);
        drawTerrain();
    });
    window.set_title("Virtual Landscape");
    window.set_size(width, height);


    // Handle mouse input (looking around the screen)
    Vec2 mouse(0,0);
    window.add_listener<MouseMoveEvent>([&](const MouseMoveEvent &m){

        // Camera control
        Vec2 delta = m.position - mouse;
        delta[1] = -delta[1];
        float sensitivity = 0.005f;
        delta = sensitivity * delta;

        yaw += delta[0];
        pitch += delta[1];

        if(pitch > PI/2.0f - 0.01f)  pitch =  PI/2.0f - 0.01f;
        if(pitch <  -PI/2.0f + 0.01f) pitch =  -PI/2.0f + 0.01f;

        Vec3 front(0,0,0);
        front[0] = sin(yaw)*cos(pitch);
        front[1] = cos(yaw)*cos(pitch);
        front[2] = sin(pitch);

        cameraFront = front.normalized();
        mouse = m.position;
    });

    // TODO: Key event listener: Handle keyboard input (moving around the screen)
    window.add_listener<KeyEvent>([&](const KeyEvent &k){

        // Movement left, right, foward and backward (WASD)
		//up
        if (k.key == GLFW_KEY_W) {
            cameraPos = cameraPos + speed * cameraFront.normalized();
        }

        //right (GLFW_KEY_D)
		
		//left(GLFW_KEY_A)
		
		//down(GLFW_KEY_S)

        // TODO: Adjust FOV -decrease FOV
        if (k.key == GLFW_KEY_UP) {
            fov -= 1.0f;
            if (fov <= 1.0f) fov = 1.0f;
        }
		
		//TODO: increase FOV for down key, max of 80

      

        // TODO: Adjust movement speed-increase
        if (k.key == GLFW_KEY_RIGHT) {
            speed += speedIncrement;
            if (speed >= 1.0f) speed = 1.0f;
        }
		
		//TODO: decrement speed on left key

       
    });

    return app.run();
}

void init(){
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // Enable seamless cubemap
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    // Enable blending
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Complile skybox shader
    skyboxShader = std::unique_ptr<Shader>(new Shader());
    skyboxShader->verbose = true;
    skyboxShader->add_vshader_from_source(skybox_vshader);
    skyboxShader->add_fshader_from_source(skybox_fshader);
    skyboxShader->link();

    // Complile terrain shader
    terrainShader = std::unique_ptr<Shader>(new Shader());
    terrainShader->verbose = true;
    terrainShader->add_vshader_from_source(terrain_vshader);
    terrainShader->add_fshader_from_source(terrain_fshader);
    terrainShader->link();

    // Get height texture (Regular fBm)
    heightTexture = std::unique_ptr<R32FTexture>(fBm2DTexture());

    // Get height texture (Hybrid Multifractal)[Optional]
    

    // Load terrain textures
    const std::string list[] = {"grass", "rock", "sand", "snow", "water", "lunar"};
    for (int i=0 ; i < 6 ; ++i) {
        loadTexture(terrainTextures[list[i]], (list[i]+".png").c_str());
        terrainTextures[list[i]]->bind();
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    // Load skybox textures
    const std::string skyList[] = {"miramar_ft", "miramar_bk", "miramar_dn", "miramar_up", "miramar_rt", "miramar_lf"};
    const std::string cubemapLayers[] = {"GL_TEXTURE_CUBE_MAP_POSITIVE_X", "GL_TEXTURE_CUBE_MAP_NEGATIVE_X",
                                         "GL_TEXTURE_CUBE_MAP_POSITIVE_Y", "GL_TEXTURE_CUBE_MAP_NEGATIVE_Y",
                                         "GL_TEXTURE_CUBE_MAP_POSITIVE_Z", "GL_TEXTURE_CUBE_MAP_NEGATIVE_Z"};

    glGenTextures(1, &skyboxTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
    int tex_wh = 1024;
    for(int i=0; i < 6; ++i) {
        std::vector<unsigned char> image;
        loadTexture(image, (skyList[i]+".png").c_str());
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0, GL_RGBA, tex_wh, tex_wh, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void genTerrainMesh() {

    // Generate a flat mesh for the terrain with given dimensions, using triangle strips
    terrainMesh = std::unique_ptr<GPUMesh>(new GPUMesh());

    // Grid resolution
    int n_width = 1024;
    int n_height = 1024;
    //int n_width = 256;
    //int n_height = 256;

    // Grid dimensions (centered at (0, 0))
    float f_width = 5.0f;
    float f_height = 5.0f;

    std::vector<Vec3> points;
    std::vector<unsigned int> indices;
    std::vector<Vec2> texCoords;

    // TODO: Generate vertex and texture coordinates
 

    // Generate element indices via triangle strips
    for(int j = 0; j < n_width - 1; ++j) {

        // Push two vertices at the base of each strip
        
        

        // Calculate subsequent vertices (innner for loop)
           

        // A new strip will begin when this index is reached
        indices.push_back(resPrim);
    }

    terrainMesh->set_vbo<Vec3>("vposition", points);
    terrainMesh->set_triangles(indices);
    terrainMesh->set_vtexcoord(texCoords);
}

void genCubeMesh() {

    // Generate a cube mesh for skybox
    skyboxMesh = std::unique_ptr<GPUMesh>(new GPUMesh());
    std::vector<Vec3> points;

    points.push_back(Vec3( 1, 1, 1)); // 0
    points.push_back(Vec3(-1, 1, 1)); // 1
    points.push_back(Vec3( 1, 1,-1)); // 2
    points.push_back(Vec3(-1, 1,-1)); // 3
    points.push_back(Vec3( 1,-1, 1)); // 4
    points.push_back(Vec3(-1,-1, 1)); // 5
    points.push_back(Vec3(-1,-1,-1)); // 6
    points.push_back(Vec3( 1,-1,-1)); // 7

    std::vector<unsigned int> indices = { 3, 2, 6, 7, 4, 2, 0, 3, 1, 6, 5, 4, 1, 0 };
    skyboxMesh->set_vbo<Vec3>("vposition", points);
    skyboxMesh->set_triangles(indices);
}

void drawSkybox() {
    skyboxShader->bind();

    // TODO: Set transformations, look vector, View matrix, Perspective matrix
    Vec3 look = cameraFront + cameraPos;
    

    // TODO: Bind Textures and set uniform. HINT: Use GL_TEXTURE0, and texture type GL_TEXTURE_CUBE_MAP with skyboxShader and set the uniform variable noiseTex.
    

    //Set atrributes and draw cube using GL_TRIANGLE_STRIP mode
    glEnable(GL_DEPTH_TEST);
    skyboxMesh->set_attributes(*skyboxShader);
    skyboxMesh->set_mode(GL_TRIANGLE_STRIP);
    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(resPrim);
    skyboxMesh->draw();

    skyboxShader->unbind();
}

void drawTerrain() {
    terrainShader->bind();

    // TODO: Generate and set Model, M matrix and set it as a uniform variable for the terainShader. You may consider an identity matrix. 
    

    // TODO: Generate and set View, V matrix and set it as a uniform variable of the terrainShader. use lookAt() function.
    Vec3 look = cameraFront + cameraPos;
  

    // TODO: Generate and set Projection, P matrix and set it as a uniform variable of the terrainShader. use OpenGP::perspective()/glm::persepctive
    

    // Set camera position
    terrainShader->set_uniform("viewPos", cameraPos);

    // Bind textures
    int i = 0;
    for (std::map<std::string, std::unique_ptr<RGBA8Texture>>::iterator it = terrainTextures.begin(); it != terrainTextures.end(); ++it) {
        glActiveTexture(GL_TEXTURE1 + i);
        (it->second)->bind();
        terrainShader->set_uniform(it->first.c_str(), 1 + i);
        ++i;
    }

    // TODO: Bind height texture to GL_TEXTURE0 and set uniform noiseTex.
    

    // Draw terrain using triangle strips
    glEnable(GL_DEPTH_TEST);
    terrainMesh->set_attributes(*terrainShader);
    terrainMesh->set_mode(GL_TRIANGLE_STRIP);
    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(resPrim);

    terrainMesh->draw();

    // Generate wave motion and set uniform wave_motion
    terrainShader->set_uniform("waveMotion", waveMotion);
    waveMotion += 0.00004f;
    if (waveMotion > 1.0f) {
        waveMotion = 0.0f;
    }

    terrainShader->unbind();
}
