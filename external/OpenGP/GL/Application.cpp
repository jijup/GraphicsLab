// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <vector>

#include "Application.h"

#include <OpenGP/MLogger.h>
#include <OpenGP/GL/gl_debug.h>


//=============================================================================
namespace OpenGP {
//=============================================================================

namespace {

    inline Application &application_wrapper(GLFWwindow *handle) {
        return *static_cast<Application*>(glfwGetWindowUserPointer(handle));
    }

    /// Callbacks

    inline void application_close_callback(GLFWwindow *handle) {
        application_wrapper(handle).close();
    }

}

Application::WindowContainer::~WindowContainer() {

    auto old_context = glfwGetCurrentContext();
    glfwMakeContextCurrent(window->handle);

    fsquad.reset();

    glfwMakeContextCurrent(old_context);
}

Application::WindowContainer::WindowContainer(Application &app, std::function<void(Window&)> user_display_callback) {

    auto display_callback = [this, user_display_callback, &app] (Window &window) {

        int new_width, new_height;
        std::tie(new_width, new_height) = window.get_size();

        float scale = window.get_pixel_scale();
        new_width *= scale;
        new_height *= scale;

        auto old_context = glfwGetCurrentContext();
        glfwMakeContextCurrent(app.hidden_window);

        if (width != new_width || height != new_height) {

            color_texture.allocate(new_width, new_height);
            depth_texture.allocate(new_width, new_height);

            framebuffer.attach_color_texture(color_texture);
            framebuffer.attach_depth_texture(depth_texture);

            width = new_width;
            height = new_height;
        }

        framebuffer.bind();
        Framebuffer::override_default(framebuffer);

        user_display_callback(window);

        Framebuffer::reset_default();
        framebuffer.unbind();

        glFlush();

        glfwMakeContextCurrent(old_context);
        glClear(GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, width, height);

        fsquad->draw_texture(color_texture);

    };

    /// After this the new window context is current
    Window *new_window = new Window(display_callback, app.hidden_window);
    window = std::unique_ptr<Window>(new_window);

    // Prevent individual calls to glfwSwapBuffers from blocking
    glfwSwapInterval(0);

    fsquad = std::unique_ptr<FullscreenQuad>(new FullscreenQuad());

    glfwMakeContextCurrent(app.hidden_window);

    std::tie(width, height) = window->get_size();

    color_texture.allocate(width, height);
    depth_texture.allocate(width, height);

    framebuffer.attach_color_texture(color_texture);
    framebuffer.attach_depth_texture(depth_texture);

}

Application::Application(const char *name) {

    // note: can be called multiple times without problems
    // docs: "additional calls to glfwInit() after successful initialization but before
    //        termination will return GL_TRUE immediately"
    if( !glfwInit() )
        mFatal() << "Failed to initialize GLFW";

    // Hint GLFW that we would like a shader-based OpenGL context
    // docs: "sets hints for the next call to glfwCreateWindow"
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Attempt to open the window: fails if required version unavailable
    // @note Intel GPUs do not support OpenGL 3.0
    if( !(hidden_window = glfwCreateWindow(800, 600, name, nullptr, nullptr)) )
        mFatal() << "Failed to open OpenGL 3 GLFW window.";

    glfwWindowHint(GLFW_VISIBLE, GL_TRUE);

    glfwMakeContextCurrent(hidden_window);
    if(glfwGetCurrentContext() != hidden_window)
        mFatal() << "Failed to make GLFW context current.";

    // GLEW Initialization (must have created a context)
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if( err != GLEW_OK )
        mFatal() << "Failed GLEW initialization";

    GLDebug::enable();

    glfwSetWindowUserPointer(hidden_window, this);
    glfwSetWindowCloseCallback(hidden_window, &application_close_callback);

    // Wipe Startup Errors (TODO: check who causes them? GLEW?)
    while (glGetError() != GL_NO_ERROR) {}

}

int Application::run() {

    running = true;

    while (running && !close_requested) {

        glfwMakeContextCurrent(hidden_window);

        for (auto &container : windows) {
            container.window->poll();
        }

        update_callback();
        ApplicationUpdateEvent event;
        send_event(event);

        for (auto &container : windows) {

            container.window->draw();

        }

        std::vector<decltype(windows.begin())> to_close;

        for (auto it = windows.begin();it != windows.end();++it)
            if ((*it).window->should_close())
                to_close.push_back(it);

        for (auto it : to_close) {
            windows.erase(it);
        }

        running = !windows.empty();

    }

    return 0;

}

Window &Application::create_window(std::function<void(Window&)> display_callback) {

    glfwMakeContextCurrent(hidden_window);

    windows.emplace_back(*this, display_callback);

    return *(windows.back().window);

}

void Application::set_update_callback(std::function<void()> fn) {
    update_callback = fn;
}

void Application::close() {
    close_requested = true;
}

//=============================================================================
} // OpenGP::
//=============================================================================
