// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "SurfaceMeshRenderer.h"


//=============================================================================
namespace OpenGP {
//=============================================================================

// vshader code
const char *SurfaceMeshRenderer::vshader() {
    return R"GLSL(

        in vec3 vposition;
        in vec3 vnormal;

        out vec3 gposition;
        out vec3 gnormal;

        void vertex() {
            gl_Position = get_MVP() * vec4(vposition, 1.0);
            gposition = (get_M() * vec4(vposition, 1.0)).xyz;
            gnormal = normalize( inverse(transpose(mat3(get_M()))) * vnormal );
        }

    )GLSL";
}

// gshader code
const char *SurfaceMeshRenderer::gshader() {
    return R"GLSL(

        // reddit.com/r/opengl/comments/34dhi7/wireframe_shader/cquax7r

        layout (triangles) in;
        layout (triangle_strip, max_vertices = 3) out;

        in vec3 gnormal[];
        in vec3 gposition[];

        out vec3 fnormal;
        out vec3 fposition;
        noperspective out vec3 fwireframe;

        void geometry() {
            for (int i = 0;i < 3;i++) {
                gl_Position = gl_in[i].gl_Position;
                fnormal = gnormal[i];
                fposition = gposition[i];
                fwireframe = vec3(0, 0, 0);
                fwireframe[i] = 1;
                geometry_vertex_shade(i);
                EmitVertex();
            }
        }

    )GLSL";
}

// fshader code
const char *SurfaceMeshRenderer::fshader() {
    return R"GLSL(

        in vec3 fnormal;
        in vec3 fposition;
        noperspective in vec3 fwireframe;

        // http://codeflow.org/entries/2012/aug/02/easy-wireframe-display-with-barycentric-coordinates
        float edgeFactor(){
            vec3 d = 1.5 * fwidth(fwireframe);
            vec3 a3 = smoothstep(vec3(0.0), d, fwireframe);
            return 1.0f - min(min(a3.x, a3.y), a3.z);
        }

        void fragment() {
            set_normal(normalize(fnormal));
            set_position(fposition);
            set_wire_weight(edgeFactor());
        }

    )GLSL";
}

SurfaceMeshRenderer::SurfaceMeshRenderer() {
    rebuild();
}

void SurfaceMeshRenderer::render(const RenderContext &context) {

    shader.bind();

    gpu_mesh.set_attributes(shader);
    update_shader(shader, context);

    if (wireframe_mode == WireframeMode::WiresOnly)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    if (!depth_test)
        glDisable(GL_DEPTH_TEST);

    if (instancing.enabled)
        gpu_mesh.draw_instanced(instancing.count);
    else
        gpu_mesh.draw();

    if (!depth_test)
        glEnable(GL_DEPTH_TEST);

    if (wireframe_mode == WireframeMode::WiresOnly)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    shader.unbind();
}

void SurfaceMeshRenderer::rebuild() {

    build_shader(shader, vshader(), fshader(), gshader());

    shader.bind();
    gpu_mesh.set_attributes(shader);
    shader.unbind();

}

void SurfaceMeshRenderer::upload_mesh(const SurfaceMesh &mesh) {

    gpu_mesh.init_from_mesh(mesh);

    shader.bind();
    gpu_mesh.set_attributes(shader);
    shader.unbind();

}

GPUMesh &SurfaceMeshRenderer::get_gpu_mesh() {
    return gpu_mesh;
}

//=============================================================================
} // OpenGP::
//=============================================================================
