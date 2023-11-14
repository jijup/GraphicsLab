// This file is free software: you can redistribute it and/or modify
// it under the terms of the GNU Library General Public License Version 2
// as published by the Free Software Foundation.
//
// This file is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with OpenGP.  If not, see <http://www.gnu.org/licenses/>.

#pragma once
#include <vector>
#include <OpenGP/GL/SceneObject.h>
#include <OpenGP/SurfaceMesh/SurfaceMesh.h>

//=============================================================================
namespace OpenGP {
//=============================================================================

class SurfaceMeshRenderShaded : public SceneObject{
private:
    SurfaceMesh& mesh;
    VertexArrayObject vao;
    ArrayBuffer<Vec3> v_buffer;
    ArrayBuffer<Vec3> n_buffer;
    ArrayBuffer<float> q_buffer;
    ElementArrayBuffer<uint> i_buffer;
    GLuint _tex; ///< Colormap Texture ID

public:
    SurfaceMeshRenderShaded(SurfaceMesh& mesh) : mesh(mesh){}
    HEADERONLY_INLINE void init();
    HEADERONLY_INLINE void display();
    HEADERONLY_INLINE Box3 bounding_box();

/// @{ color quality mapping
private:
    bool _use_colormap = false;
    Scalar _colormap_min = 0.0f;
    Scalar _colormap_max = 1.0f;
public:
    HEADERONLY_INLINE void colormap_enabled(bool);
    HEADERONLY_INLINE void colormap_set_range(Scalar min, Scalar max);
/// @}
};

//=============================================================================
} // namespace OpenGP
//=============================================================================

#ifdef HEADERONLY
    #include "SurfaceMeshRenderShaded.cpp"
#endif
