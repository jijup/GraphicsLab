// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <vector>
#include <Eigen/Core>
#include <OpenGP/types.h>
#include <OpenGP/MLogger.h>

#include <OpenGP/GL/VertexArrayObject.h>
#include <OpenGP/GL/Shader.h>
#include <OpenGP/GL/Buffer.h>

//=============================================================================
namespace OpenGP {
//=============================================================================

class SceneObject{
// TODO: split public/private
public:
    Shader program;
    /// Model Transformation Matrix
    Mat4x4 model = Mat4x4::Identity();
    /// Per-object color
    /// TODO upload to shader
    Vec3 color = Vec3(.6,.6,.6);

    virtual ~SceneObject(){}
    virtual void init() = 0;
    virtual void display() = 0;
    
    /// The bounding box can be used to automatically compute the parameters
    /// of the camera, so every drawable object has to define it.
    virtual Box3 bounding_box() = 0;
};

//=============================================================================
} // namespace OpenGP
//=============================================================================
