// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <OpenGP/GL/MaterialRenderer.h>
#include <OpenGP/GL/GPUMesh.h>
#include <OpenGP/SurfaceMesh/SurfaceMesh.h>


//=============================================================================
namespace OpenGP {
//=============================================================================

class SurfaceMeshRenderer : public MaterialRenderer {
private:

    GPUMesh gpu_mesh;

    Shader shader;

    HEADERONLY_INLINE static const char *vshader();
    HEADERONLY_INLINE static const char *gshader();
    HEADERONLY_INLINE static const char *fshader();

public:

    bool depth_test = true;
    struct {
        bool enabled = false;
        int count = 1;
    } instancing;

    HEADERONLY_INLINE SurfaceMeshRenderer();

    virtual ~SurfaceMeshRenderer() {}

    HEADERONLY_INLINE void render(const RenderContext&);

    HEADERONLY_INLINE void rebuild();

    HEADERONLY_INLINE void upload_mesh(const SurfaceMesh &mesh);

    HEADERONLY_INLINE GPUMesh &get_gpu_mesh();

};

//=============================================================================
} // OpenGP::
//=============================================================================

#ifdef HEADERONLY
    #include "SurfaceMeshRenderer.cpp"
#endif
