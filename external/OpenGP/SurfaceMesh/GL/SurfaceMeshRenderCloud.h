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
#include <OpenGP/MLogger.h>
#include <OpenGP/GL/SceneGraph.h>
#include <OpenGP/GL/PointsRenderer.h>
#include <OpenGP/SurfaceMesh/SurfaceMesh.h>
#include <OpenGP/SurfaceMesh/Eigen.h>
#include <OpenGP/SurfaceMesh/bounding_box.h>

//=============================================================================
namespace OpenGP {
//=============================================================================

class SurfaceMeshRenderCloud : public PointsRenderer{
private:
    SurfaceMesh& mesh;
public:
    SurfaceMeshRenderCloud(SurfaceMesh& mesh) : mesh(mesh){
        Mat3xN mat = vertices_matrix(mesh);
        PointsRenderer::load(mat);
    }
    Box3 bounding_box(){ return OpenGP::bounding_box(mesh); }
};

//=============================================================================
} // namespace OpenGP
//=============================================================================
