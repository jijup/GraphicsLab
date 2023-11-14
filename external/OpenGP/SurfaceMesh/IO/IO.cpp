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

#include <OpenGP/SurfaceMesh/SurfaceMesh.h>
#include <OpenGP/SurfaceMesh/IO/IO.h>
#include <clocale>
#include <locale>
#include <algorithm>
#include <string>

//== NAMESPACE ================================================================
namespace OpenGP {
//=============================================================================

namespace{
inline char easytolower(char in){
    static std::locale locale;
    return std::tolower<char>(in, locale);
}
} // ::anonymous

bool read_mesh(SurfaceMesh& mesh, const std::string& filename)
{
    std::setlocale(LC_NUMERIC, "C");

    // clear mesh before reading from file
    mesh.clear();

    // extract file extension
    std::string::size_type dot(filename.rfind("."));
    if (dot == std::string::npos) return false;
    std::string ext = filename.substr(dot+1, filename.length()-dot-1);
    std::transform(ext.begin(), ext.end(), ext.begin(), easytolower);

    // extension determines reader
    if (ext == "off")
    {
        return read_off(mesh, filename);
    }
    else if (ext == "obj")
    {
        return read_obj(mesh, filename);
    }
    else if (ext == "stl")
    {
        return read_stl(mesh, filename);
    }

    // we didn't find a reader module
    return false;
}


//-----------------------------------------------------------------------------


bool write_mesh(const SurfaceMesh& mesh, const std::string& filename)
{
    // extract file extension
    std::string::size_type dot(filename.rfind("."));
    if (dot == std::string::npos) return false;
    std::string ext = filename.substr(dot+1, filename.length()-dot-1);
    std::transform(ext.begin(), ext.end(), ext.begin(), easytolower);


    // extension determines reader
    if (ext == "off")
    {
        return write_off(mesh, filename);
    }
    else if(ext=="obj")
    {
        return write_obj(mesh, filename);
    }

    // we didn't find a writer module
    return false;
}

//=============================================================================
} // namespace OpenGP
//=============================================================================
