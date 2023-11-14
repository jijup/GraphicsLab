// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <string>

#include <OpenGP/headeronly.h>
#include <OpenGP/types.h>
#include <OpenGP/GL/Eigen.h>
#include <OpenGP/GL/Shader.h>
#include <OpenGP/GL/Material.h>

//=============================================================================
namespace OpenGP {
//=============================================================================

struct RenderContext {

    // Camera parameters

    float aspect;
    float vfov;       // Only relevant for prespective
    float view_width; // only relevant for orthographic

    float near;
    float far;

    Vec3 eye;
    Vec3 forward;
    Vec3 up;

    bool ortho = false;

    // Object parameters

    Vec3 translation;
    Vec3 scale;
    Quaternion rotation;

    // Matrices

    Mat4x4 M;
    Mat4x4 V;
    Mat4x4 P;
    Mat4x4 MV;
    Mat4x4 VP;
    Mat4x4 MVP;

    void update_model() {
        auto T = translate(translation.x(),translation.y(),translation.z());
        Mat4x4 R = Mat4x4::Identity();
        R.block<3, 3>(0,0) = rotation.matrix();
        R(3, 3) = 1;
        auto S = OpenGP::scale(scale.x(),scale.y(),scale.z());
        M = T * R * S;

        MV = V * M;
        MVP = P * V * M;
    }

    void update_view() {

        V = look_at(eye, Vec3(eye + forward), up);

        MV = V * M;
        VP = P * V;
        MVP = P * V * M;
    }

    void update_projection() {

        if (ortho) {
            P = OpenGP::ortho(-view_width/2, view_width/2, -view_width/(2*aspect), view_width/(2*aspect), near, far);
        } else {
            P = perspective(vfov, aspect, near, far);
        }

        VP = P * V;
        MVP = P * V * M;
    }

};

enum class WireframeMode {
    None,
    Overlay,
    WiresOnly
};

class MaterialRenderer {
protected:

    Material material;

    HEADERONLY_INLINE void build_shader(Shader &shader, const std::string &vshader, const std::string &fshader, const std::string &gshader = "");

    HEADERONLY_INLINE void update_shader(Shader &shader, const RenderContext &context);

public:

    WireframeMode wireframe_mode = WireframeMode::None;
    Vec3 wirecolor = Vec3(0, 0, 0);
    bool use_wirecolor = true;

    MaterialRenderer() {}

    virtual ~MaterialRenderer() {}

    virtual void render(const RenderContext&) = 0;

    virtual void rebuild() {}

    HEADERONLY_INLINE void set_material(const Material &material);

    HEADERONLY_INLINE Material &get_material();
    HEADERONLY_INLINE const Material &get_material() const;

};

//=============================================================================
} // OpenGP::
//=============================================================================

#ifdef HEADERONLY
    #include "MaterialRenderer.cpp"
#endif
