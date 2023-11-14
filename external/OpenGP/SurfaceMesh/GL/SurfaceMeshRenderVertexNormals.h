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
#include <OpenGP/GL/SegmentsRenderer.h>
#include <OpenGP/SurfaceMesh/SurfaceMesh.h>
#include <OpenGP/SurfaceMesh/Eigen.h>
#include <OpenGP/SurfaceMesh/bounding_box.h>

//=============================================================================
namespace OpenGP {
//=============================================================================

class SurfaceMeshRenderVertexNormals : public SegmentsRenderer{
protected:
    SurfaceMesh& mesh;

public:
    SurfaceMeshRenderVertexNormals(SurfaceMesh& mesh, Scalar offset=.1) : mesh(mesh){
        this->color = Vec3(0,0,1); ///< blue normals
        Mat3xN P1 = OpenGP::vertices_matrix(mesh);
        Mat3xN N = OpenGP::normals_matrix(mesh);
        Mat3xN P2 = P1 + offset*N;
        SegmentsRenderer::load(P1,P2);
    }
    Box3 bounding_box(){ return OpenGP::bounding_box(mesh); }
};

//=============================================================================
} // namespace OpenGP
//=============================================================================
