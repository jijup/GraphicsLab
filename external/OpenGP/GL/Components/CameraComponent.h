// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <OpenGP/GL/Scene.h>
#include <OpenGP/GL/Entity.h>
#include <OpenGP/GL/Components/WorldRenderComponent.h>
#include <OpenGP/GL/Components/TransformComponent.h>
#include <OpenGP/GL/MaterialRenderer.h>
#include <OpenGP/GL/Window.h>
#include <OpenGP/MLogger.h>


//=============================================================================
namespace OpenGP {
//=============================================================================

/// An event indicating that any canvases should draw their UI elements
struct GUIRenderEvent {};

/// A component representing a camera in the scene
class CameraComponent : public Component, public EventProvider {
private:

    Window *window = nullptr;

public:

    /// The distance from the eye to the near clipping plane
    float near_plane = 0.1f;

    /// The distance from the eye to the far clipping plane
    float far_plane = 1000;

    /// The vertical field-of-view of the camera in degrees
    float vfov = 60;

    CameraComponent() {}

    void init() {
        require<TransformComponent>();
    }

    /// Check if there is a window associated with the camera
    bool has_window() const {
        return window != nullptr;
    }

    /// Get the view matrix of the camera
    Mat4x4 get_view() const {
        auto &t = get<TransformComponent>();
        return look_at(t.position, Vec3(t.position + t.forward()), t.up());
    }

    /// Get the projection matrix of the camera used when rendering to the attached window
    Mat4x4 get_projection() const {
        int width, height;
        std::tie(width, height) = window->get_size();
        return get_projection(width, height);
    }

    /// @brief Get the projection matrix of the camera used when rendering to a
    /// framebuffer with width `width` and height `height`
    Mat4x4 get_projection(int width, int height) const {
        return perspective(vfov, (float)width / (float)height, near_plane, far_plane);
    }

    /// Draw the scene from the camera POV assuming there is a window attached and bound
    void draw() {

        if (window == nullptr) {
            mFatal() << "Camera window not set";
        }

        int width, height;
        std::tie(width, height) = window->get_size();

        draw(width, height);

    }

    /// Draw the scene from the camera POV into the bound framebuffer with given dimensions
    void draw(int width, int height) {

        RenderContext context;

        if (has_window()) {
          float scale = window->get_pixel_scale();
          width *= scale;
          height *= scale;
        }
        glViewport(0, 0, width, height);
        glClearColor(0.15f, 0.15f, 0.15f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        context.aspect = (float)width / (float)height;
        context.vfov = vfov;
        context.near = near_plane;
        context.far = far_plane;
        context.eye = get<TransformComponent>().position;
        context.forward = get<TransformComponent>().forward();
        context.up = get<TransformComponent>().up();

        context.update_view();
        context.update_projection();

        for (auto &renderable : get_scene().all_of_type<WorldRenderComponent>()) {

            if (!renderable.visible)
                continue;

            auto &transform = renderable.get<TransformComponent>();

            context.translation = transform.position;
            context.scale = transform.scale;
            context.rotation = transform.rotation;

            context.update_model();

            renderable.get_renderer().render(context);
        }

        GUIRenderEvent event;

        send_event(event);

    }

    /// Get the attached window assuming it exists
    Window &get_window() {
        return *window;
    }

    /// Attach a window to the camera
    void set_window(Window &window) {
        this->window = &window;
    }

};

//=============================================================================
} // OpenGP::
//=============================================================================
