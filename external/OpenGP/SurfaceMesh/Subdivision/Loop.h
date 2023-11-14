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
#include <OpenGP/headeronly.h>
#include <OpenGP/SurfaceMesh/Algorithm.h>

class SurfaceMeshSubdivideLoop : public OpenGP::SurfaceMeshAlgorithm{
public:
    static HEADERONLY_INLINE void exec(OpenGP::SurfaceMesh& mesh);
};

#ifdef HEADERONLY
    #include "Loop.cpp"
#endif
