#include "utility.h"
#include "Skybox.h"
#include "Camera.h"
#include "Mesh.h"

using namespace OpenGP;
const int width = 1000, height = 1000;

int main(int, char**) {

    Application app;

    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);     // Make skybox seamless
    glEnable(GL_DEPTH_TEST);     // enable depth test for skybox and so on
    
    Vec3 skyColor = Vec3(0.6, 0.7, 0.8);
    Vec3 lightPos = Vec3(30.0f, 30.0f, 30.0f);
    

    Skybox skybox(skyColor);    
    Camera camera(width, height);
	Mesh mesh;
	mesh.init();


    // Display callback
    Window& window = app.create_window([&](Window&) {
        float time = glfwGetTime();                
        glViewport(0, 0, width, height);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
        mesh.draw(camera, time);
        skybox.draw(camera, time);
        
        });
    window.set_title("3D Mesh");
    window.set_size(width, height);

    Vec2 mouse(0, 0);
    window.add_listener<MouseMoveEvent>([&](const MouseMoveEvent& m) {
        Vec2 changeInMousePosition = m.position - mouse;
        camera.updateCameraAngles(changeInMousePosition);
        mouse = m.position;
        });

    window.add_listener<KeyEvent>([&](const KeyEvent& k) {
        camera.updateCamera(k);
        });

    return app.run();
}








