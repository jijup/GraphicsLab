// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <list>
#include <memory>

#include <OpenGP/headeronly.h>
#include <OpenGP/GL/Window.h>
#include <OpenGP/GL/Framebuffer.h>
#include <OpenGP/GL/FullscreenQuad.h>


//=============================================================================
namespace OpenGP {
//=============================================================================

/// An event sent every frame by the application
struct ApplicationUpdateEvent {};

/// A framework class that manages windows and runs the main loop
class Application : public EventProvider {
private:

    struct WindowContainer {

        int width, height;

        RGB8Texture color_texture;
        D16Texture depth_texture;
        Framebuffer framebuffer;

        std::unique_ptr<Window> window;
        std::unique_ptr<FullscreenQuad> fsquad;

        HEADERONLY_INLINE WindowContainer(Application &app, std::function<void(Window&)> user_display_callback);

        HEADERONLY_INLINE ~WindowContainer();

    };

    GLFWwindow *hidden_window;

    std::list<WindowContainer> windows;

    bool running = false;
    bool close_requested = false;

    std::function<void()> update_callback = [](){};

public:

    /// Create a new application object
    HEADERONLY_INLINE Application(const char *name="OpenGP Application");

    /// Start the application
    HEADERONLY_INLINE int run();

    /// Open a new window as part of the application
    HEADERONLY_INLINE Window &create_window(std::function<void(Window&)> display_callback);

    /// @brief Provide a callback to be executed every frame
    /// @deprecated use `ApplicationUpdateEvent` instead
    HEADERONLY_INLINE void set_update_callback(std::function<void()> fn);

    /// Set the application's close flag
    HEADERONLY_INLINE void close();

};

//=============================================================================
} // OpenGP::
//=============================================================================

#ifdef HEADERONLY
    #include "Application.cpp"
#endif
