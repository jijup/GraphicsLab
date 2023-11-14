#include <fstream>

#include <OpenGP/GL/Application.h>
#include <OpenGP/GL/Eigen.h>
#include <OpenGP/SurfaceMesh/SurfaceMesh.h>


using namespace OpenGP;

constexpr int width = 1024;
constexpr int height = 768;

std::unique_ptr<Shader> shader;
std::unique_ptr<GPUMesh> mesh;

float t = 0.0f;

void draw() {

    glViewport(0, 0, width, height);

    // Depth testing is disabled by default, so make sure you turn it on before
    // trying to draw any 3D objects
    glEnable(GL_DEPTH_TEST);

    // Note that we now clear the depth buffer as well
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader->bind();

    Mat4x4 model = Mat4x4::Identity();

    // Now that we are drawing a 3D scene, we need to define the paramters of
    // the camera, starting with its position
    Vec3 eye = Vec3(5 * cos(t), 2, 5 * sin(t));

    // This helper function generates a view matrix for us
    Mat4x4 view = look_at(eye,
                          Vec3(0, 1, 0),
                          Vec3(0, 1, 0));

    // To build the projection matrix we need the window's aspect ratio
    float ratio = float(width) / float(height);

    // Another helper function to build the projection matrix
    Mat4x4 projection = perspective(60, ratio, 0.1, 50);

    // The combined model, view, and projection transformation matrix
    Mat4x4 MVP = projection * view * model;

    // We need to apply any rotation from the model matrix to the vertex normals
    // while ignoring any translation or scaling
    Mat4x4 model_rotation = model.inverse().transpose();

    // Pass some camera related values that we need in the shaders
    shader->set_uniform("model", model);
    shader->set_uniform("model_rotation", model_rotation);
    shader->set_uniform("MVP", MVP);
    shader->set_uniform("eye", eye);

    // Set the material and lighting paramters
    Vec3 light_dir = Vec3(cos(2*t), -1, sin(2*t)).normalized();
    shader->set_uniform("light_dir", light_dir);
    shader->set_uniform("light_color", Vec3(1.0, 0.9, 0.8));
    shader->set_uniform("ambient_light", Vec3(0.05, 0.15, 0.35));
    shader->set_uniform("bunny_color", Vec3(0.9, 0.3, 0.1));
    shader->set_uniform("bunny_specular", Vec3(0.6, 0.6, 0.6));
    shader->set_uniform("specular_power", 50.0f);

    mesh->set_attributes(*shader);

    mesh->draw();

    shader->unbind();

}

void update() { t += 0.0001f; }

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

    // Create a SurfaceMesh object to read from a .obj file
    SurfaceMesh mesh_data;
    mesh_data.read("bunny.obj");
    mesh_data.update_vertex_normals();

    // Upload the mesh data to the GPU
    mesh->init_from_mesh(mesh_data);

    shader->add_vshader_from_source(load_source("vert.glsl").c_str());
    shader->add_fshader_from_source(load_source("frag.glsl").c_str());
    shader->link();

    app.add_listener<ApplicationUpdateEvent>([](const ApplicationUpdateEvent&){
        update();
    });

    Window& window = app.create_window([](Window&){ draw(); });
    window.set_title("Phong Shading Example");
    window.set_size(width, height);

    return app.run();

}
