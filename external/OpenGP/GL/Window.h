// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <string>
#include <vector>
#include <tuple>
#include <functional>

#include <OpenGP/headeronly.h>
#include <OpenGP/types.h>
#include <OpenGP/GL/EventProvider.h>
#include <OpenGP/GL/glfw.h>


//=============================================================================
namespace OpenGP {
//=============================================================================

typedef int KeyCode;

/// An event indicating that the status of a keyboard key changed
struct KeyEvent {
    /// The keycode of the affected key
    KeyCode key;
    /// Was the key released or pressed
    bool released;
};

/// An event indicating that the mouse was moved
struct MouseMoveEvent {
    /// The relative movement of the mouse
    Vec2 delta;
    /// The new position of the mouse
    Vec2 position;
};

/// An event indicating that the status of a mouse button changed
struct MouseButtonEvent {
    /// The index of the affected button
    int button;
    /// Was the button released or pressed
    bool released;
};

/// An event indicating that scroll input was recieved
struct MouseScrollEvent {
    /// The scroll displacement
    Vec2 delta;
};

class Application;


/// A wrapper object that manages a window
class Window : public EventProvider {

    friend class Application;

private:

    GLFWwindow* handle = nullptr;

    bool close_flag = false;

    std::vector<std::function<void()>> input_actions;

    std::function<void(Window&)> display_callback = [](Window&){};

    HEADERONLY_INLINE static void mouse_button_callback(GLFWwindow *handle, int button, int action, int mods);
    HEADERONLY_INLINE static void mouse_position_callback(GLFWwindow *handle, double x, double y);
    HEADERONLY_INLINE static void mouse_enter_callback(GLFWwindow *handle, int entered);
    HEADERONLY_INLINE static void mouse_scroll_callback(GLFWwindow *handle, double dx, double dy);
    HEADERONLY_INLINE static void key_callback(GLFWwindow *handle, int key, int scancode, int action, int mods);
    HEADERONLY_INLINE static void char_callback(GLFWwindow *handle, unsigned int codepoint);

public:

    /// Create and show a new window
    HEADERONLY_INLINE Window(std::function<void(Window&)> display_callback);

    /// Create and show a new window that shares objects with a parent context
    HEADERONLY_INLINE Window(std::function<void(Window&)> display_callback, GLFWwindow *parent_context);

    Window(const Window&) = delete;
    Window &operator=(const Window&) = delete;

    HEADERONLY_INLINE virtual ~Window();

    /// Get the ratio of window size to framebuffer size
    HEADERONLY_INLINE float get_pixel_scale();

    /// Set the caption of the window that is shown on the desktop
    HEADERONLY_INLINE void set_title(const std::string &title);

    /// Get the size in pixels of the window as reported by the OS
    HEADERONLY_INLINE std::tuple<int, int> get_size();

    /// Resize the window
    HEADERONLY_INLINE void set_size(int width, int height);

    /// Get the version number of the window's OpenGL context
    HEADERONLY_INLINE std::tuple<int, int, int> get_GL_version();

    /// Execute the display callback and swap the buffers to update the window contents
    HEADERONLY_INLINE void draw();

    /// Process any pending input actions
    HEADERONLY_INLINE void poll();

    /// Retrieve the window's close flag
    HEADERONLY_INLINE bool should_close() const;

    /// Set the window's close flag
    HEADERONLY_INLINE void close();

    /// Mark the mouse as captured
    HEADERONLY_INLINE void capture_mouse();

    /// Mark the keyboard as captured
    HEADERONLY_INLINE void capture_keyboard();

    /// Unset the capture flag for the mouse
    HEADERONLY_INLINE void release_mouse();

    /// Unset the capture flag for the keyboard
    HEADERONLY_INLINE void release_keyboard();

    /// A class that exposes the status of user input for a particular window
    class Input {

        friend class Window;

    private:

        std::unordered_map<int, bool> buttons;
        std::unordered_map<KeyCode, bool> keys;
        std::u32string text;

    public:

        /// Flag indicating whether a specific element is handling mouse input
        bool mouse_captured;

        /// Flag indicating whether a specific element is handling keyboard input
        bool keyboard_captured;

        /// The current position of the mouse in the window
        Vec2 mouse_position;

        /// The change in mouse position since the last input poll
        Vec2 mouse_delta;

        /// The scroll displacement since last input poll
        Vec2 mouse_scroll_delta;

        /// Get the status (pressed or not) of the specified mouse button
        bool get_mouse(int button) const {
            auto it = buttons.find(button);
            if (it == buttons.end()) {
                return false;
            } else {
                return (*it).second;
            }
        }

        /// Get the status (pressed or not) of the specified keyboard key
        bool get_key(KeyCode key) const {
            auto it = keys.find(key);
            if (it == keys.end()) {
                return false;
            } else {
                return (*it).second;
            }
        }

        /// Get text entered since last input poll
        const std::u32string &get_text() const {
            return text;
        }

    };

private:

    Input input;

public:

    /// Get a reference to this window's input manager
    const Input &get_input() const { return input; }

};

/// An event indicating that this window is closing
struct WindowCloseEvent {
    Window &window;
};

//=============================================================================
} // namespace OpenGP
//=============================================================================

#ifdef HEADERONLY
    #include "Window.cpp"
#endif
