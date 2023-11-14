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
#include <OpenGP/SurfaceMesh/SurfaceMesh.h>
#include <string>

//=============================================================================
namespace OpenGP {
//=============================================================================

HEADERONLY_INLINE bool read_mesh(SurfaceMesh& mesh, const std::string& filename);
HEADERONLY_INLINE bool read_off(SurfaceMesh& mesh, const std::string& filename);
HEADERONLY_INLINE bool read_obj(SurfaceMesh& mesh, const std::string& filename);
HEADERONLY_INLINE bool read_stl(SurfaceMesh& mesh, const std::string& filename);
HEADERONLY_INLINE bool write_mesh(const SurfaceMesh& mesh, const std::string& filename);
HEADERONLY_INLINE bool write_off(const SurfaceMesh& mesh, const std::string& filename);
HEADERONLY_INLINE bool write_obj(const SurfaceMesh& mesh, const std::string& filename);

/// Private helper function
template <typename T> void read(FILE* in, T& t)
{
    size_t n_items(0);
    n_items = fread((char*)&t, 1, sizeof(t), in);
    assert(n_items > 0);
}

//=============================================================================
} // namespace OpenGP
//=============================================================================

#ifdef HEADERONLY
    #include "IO.cpp"
    #include "IO_obj.cpp"
    #include "IO_off.cpp"
    #include "IO_poly.cpp"
    #include "IO_stl.cpp"
#endif
