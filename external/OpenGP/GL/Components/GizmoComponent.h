// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <OpenGP/GL/Entity.h>
#include <OpenGP/GL/Components/TransformComponent.h>
#include <OpenGP/GL/Components/GUICanvasComponent.h>


//=============================================================================
namespace OpenGP {
//=============================================================================

/// A component that draws a gizmo that allows modification of the entity's transform
class GizmoComponent : public Component {
public:

    /// The current operation mode of the gizmo
    ImGuizmo::OPERATION operation = ImGuizmo::TRANSLATE;

    void init() {}

    /// Handler for gui events that creates the gizmo UI element
    void on_gui(const GUIElementDrawEvent &event) {

        auto &input = event.camera.get_window().get_input();

        if (input.get_key(GLFW_KEY_T)) {
            operation = ImGuizmo::TRANSLATE;
        } else if (input.get_key(GLFW_KEY_R)) {
            operation = ImGuizmo::ROTATE;
        } else if (input.get_key(GLFW_KEY_S)) {
            operation = ImGuizmo::SCALE;
        }

        auto &transform = get<TransformComponent>();

        Mat4x4 proj = event.camera.get_projection();
        Mat4x4 view = event.camera.get_view();
        Mat4x4 model = transform.get_transformation_matrix();
        Mat4x4 dmodel = Mat4x4::Zero();

        ImGuizmo::Manipulate(view.data(), proj.data(), operation, ImGuizmo::LOCAL, model.data(), dmodel.data());

        switch(operation) {
            case ImGuizmo::TRANSLATE: transform.apply_translation_matrix(dmodel); break;
            case ImGuizmo::ROTATE:    transform.apply_rotation_matrix(dmodel); break;
            case ImGuizmo::SCALE:     transform.set_scale_matrix(model); break;
        }


    }

};

//=============================================================================
} // OpenGP::
//=============================================================================
