// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <OpenGP/SurfaceMesh/SurfaceMesh.h>
#include <OpenGP/GL/SyntheticDepthmap.h>


//=============================================================================
namespace OpenGP {
//=============================================================================

class SurfaceMeshDepthmapRenderer : public DepthmapRenderer {
private:

    const char* vshader = R"GLSL(
        #version 330 core

        uniform mat4 M;
        uniform mat4 V;
        uniform mat4 P;

        in vec3 vposition;
        out vec3 fpos;

        void main(){
            vec4 pos = V * M * vec4(vposition, 1);
            fpos = pos.xyz;

            gl_Position = P * pos;
        }
    )GLSL";

    const char* fshader = R"GLSL(
        #version 330 core

        in vec3 fpos;
        out vec4 fcolor;

        void main(){
            fcolor = vec4(fpos, 1);
        }
    )GLSL";

    Shader shader;

    GPUMesh gpu_mesh;

public:

    SurfaceMeshDepthmapRenderer() {
        shader.add_vshader_from_source(vshader);
        shader.add_fshader_from_source(fshader);
        shader.link();
    }

    SurfaceMeshDepthmapRenderer(const SurfaceMesh &mesh) : SurfaceMeshDepthmapRenderer() {
        upload_mesh(mesh);
    }

    // DEPRECATED
    void init() {}

    void upload_mesh(const SurfaceMesh &mesh) {
        gpu_mesh.init_from_mesh(mesh);
    }

    void draw() {

        glEnable(GL_DEPTH_TEST);

        gpu_mesh.set_attributes(shader);

        gpu_mesh.draw();

    }

    Shader &get_shader() {
        return shader;
    }

};

//=============================================================================
} // namespace OpenGP
//=============================================================================
