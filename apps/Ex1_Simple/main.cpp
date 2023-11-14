#include <OpenGP/GL/Application.h>


using namespace OpenGP;

// The width and height of our window
constexpr int width = 1024;
constexpr int height = 768;

void draw() {

    // Code here gets called every frame beacuse we passed this function to
    // app.create_window() in main()
    //
    // Make sure to do all draw calls during the execution of this function, not
    // before or after, to avoid any conflicts with internal OpenGP logic

    glViewport(0, 0, width, height);

    glClearColor(1.0f, 0.5f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

}

int main(int argc, char** argv) {

    // Create a new application
    //
    // Make sure no OpenGL code runs before this line (IMPORTANT), including
    // wrapper object constructors, as the Application constructor initializes
    // OpenGL for you
    Application app;

    // Here we ask the application to spawn a new window
    //
    // Notice that we keep only a reference to the window object, as it is
    // owned by the application and can not be copied/moved
    Window& window = app.create_window([](Window&){ draw(); });

    // The window reference can be used to manipulate the window properties
    window.set_title("Simple Window Opening Example");
    window.set_size(width, height);

    // Finally, give control of the main loop to OpenGP by running the app
    return app.run();

}
