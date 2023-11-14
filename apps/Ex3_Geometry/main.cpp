#include <fstream>

#include <OpenGP/GL/Application.h>


using namespace OpenGP;

constexpr int width = 1024;
constexpr int height = 768;

std::unique_ptr<Shader> shader;

// A GPUMesh object
//
// This object wraps a Vertex Array Object and associated Vertex Buffer Objects
// which together define the geometry of a mesh
std::unique_ptr<GPUMesh> mesh;

float t = 0.0f;

void draw() {

    glViewport(0, 0, width, height);

    // We now need to clear every frame as we do not draw to the entire screen
    // every time: try disabling this and see what happens
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader->bind();

    // This time pass in a rotation matrix that we build on the CPU
    Mat4x4 model;
    model << cos(t),  sin(t), 0, 0,
             -sin(t), cos(t), 0, 0,
             0,       0,      1, 0,
             0,       0,      0, 1;

    // Upload the matrix to the GPU
    shader->set_uniform("model", model);

    // Apply our custom attribute mapping
    mesh->set_attributes(*shader);

    // Launch the draw call for the mesh
    mesh->draw();

    shader->unbind();

}

void update() { t += 0.001f; }

std::string load_source(const char* fname) {
    std::ifstream f(fname);
    std::stringstream buffer;
    buffer << f.rdbuf();
    return buffer.str();
}

int main(int argc, char** argv) {

    Application app;

    shader = std::unique_ptr<Shader>(new Shader());
    mesh = std::unique_ptr<GPUMesh>(new GPUMesh());

    // Define some custom vertex data
    std::vector<Vec3> custom_vertex_data = {
        Vec3(-0.5, -0.5, 0),
        Vec3(-0.5,  0.5, 0),
        Vec3(0.5,  -0.5, 0),
        Vec3(0.5,   0.5, 0)
    };

    // Store our custom data in the mesh as a vertex attribute
    mesh->set_vbo<Vec3>("custom_vert_attrib", custom_vertex_data);

    // Define a mapping from attributes to triangle
    //
    // Note that GPUMesh uses glDrawElements rather than glDrawArrays, and so
    // requires that you provide indices into the VBOs that define where to get
    // the attribute data for each vertex
    //
    // This approach is more flexible, as the attribute data does not need to be
    // sequential
    std::vector<unsigned int> triangle_indices = {
        0, 1, 2, // Every group of three defines a triangle
        1, 2, 3
    };

    // Store the triangle indices in the mesh object
    mesh->set_triangles(triangle_indices);

    shader->add_vshader_from_source(load_source("vert.glsl").c_str());
    shader->add_fshader_from_source(load_source("frag.glsl").c_str());
    shader->link();

    app.add_listener<ApplicationUpdateEvent>([](const ApplicationUpdateEvent&){
        update();
    });

    Window& window = app.create_window([](Window&){ draw(); });
    window.set_title("Simple Geometry Example");
    window.set_size(width, height);

    return app.run();

}
