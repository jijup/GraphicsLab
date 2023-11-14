#include <fstream>

#include <OpenGP/GL/Application.h>


using namespace OpenGP;

constexpr int width = 1024;
constexpr int height = 768;

// A shader wrapper object
//
// Note that we do not declare the shader as a simple global variable. The
// reason for this is that global variables are initialized before main() runs,
// which would cause the Shader() constructor (containing OpenGL calls) to run
// before OpenGL is initialized in main() by the Application() constructor
std::unique_ptr<Shader> shader;

// A FullscrenQuad object
//
// This encapsulates all the OpenGL logic needed to draw a quad that covers the
// screen, which is commonly used for copying images and applying effects
std::unique_ptr<FullscreenQuad> fsquad;

// A global variable to keep track of time
float t = 0.0f;

void draw() {

    // Tell OpenGL where in the window we are drawing
    glViewport(0, 0, width, height);

    // Bind the shader object so we can use it
    shader->bind();

    // Set a global input for the shader
    shader->set_uniform("time", t);

    // Map the vertex data from fsquad to the shader inputs
    //
    // The FullscreenQuad class provides the "vposition" attribute by default
    fsquad->set_attributes(*shader);

    // Launch the draw call for the quad
    fsquad->draw();

    // Unbind the shader so it is not accidentally used elsewhere
    shader->unbind();

}

void update() {

    // We can use this function that we registered in main() to do per-frame
    // logic that is not associated with a particular window

    t += 0.01f;

}

// Helper function for loading files (https://stackoverflow.com/a/2602258)
std::string load_source(const char* fname) {
    std::ifstream f(fname);
    std::stringstream buffer;
    buffer << f.rdbuf();
    return buffer.str();
}

int main(int argc, char** argv) {

    Application app;

    // Initialize the globally-accessible shader and fsquad objects now that
    // it is safe to make OpenGL calls
    shader = std::unique_ptr<Shader>(new Shader());
    fsquad = std::unique_ptr<FullscreenQuad>(new FullscreenQuad());

    // Compile and link the shader program from the above sources
    shader->add_vshader_from_source(load_source("vert.glsl").c_str());
    shader->add_fshader_from_source(load_source("frag.glsl").c_str());
    shader->link();

    // Here we ask the application to call a function every frame
    app.add_listener<ApplicationUpdateEvent>([](const ApplicationUpdateEvent&){
        update();
    });

    Window& window = app.create_window([](Window&){ draw(); });
    window.set_title("Minimal Shader Example");
    window.set_size(width, height);

    return app.run();

}
