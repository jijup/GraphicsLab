// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <string>
#include <functional>
#include <unordered_map>

#include <OpenGP/types.h>
#include <OpenGP/headeronly.h>
#include <OpenGP/GL/Shader.h>

//=============================================================================
namespace OpenGP{
//=============================================================================

class Material {
private:

    std::string vertex_code;
    std::string geometry_code;
    std::string fragment_code;

    std::unordered_map<std::string, std::function<void(Shader &shader)>> properties;

    HEADERONLY_INLINE static const char *default_vertex_code();
    HEADERONLY_INLINE static const char *default_geometry_code();
    HEADERONLY_INLINE static const char *default_fragment_code();

public:

    HEADERONLY_INLINE Material();
    HEADERONLY_INLINE Material(const std::string &fragment_code);
    HEADERONLY_INLINE Material(const std::string &vertex_code, const std::string &geometry_code, const std::string &fragment_code);

    HEADERONLY_INLINE const std::string &get_vertex_code() const;
    HEADERONLY_INLINE const std::string &get_geometry_code() const;
    HEADERONLY_INLINE const std::string &get_fragment_code() const;

    HEADERONLY_INLINE void set_vertex_code(const std::string& code);
    HEADERONLY_INLINE void set_geometry_code(const std::string& code);
    HEADERONLY_INLINE void set_fragment_code(const std::string& code);

    template <typename T>
    void set_property(const std::string &name, const T &val) {
        properties[name] = [val, name] (Shader &shader) {
            shader.set_uniform(name.c_str(), val);
        };
    }

    HEADERONLY_INLINE void apply_properties(Shader &shader) const;

};

//=============================================================================
} // OpenGP::
//=============================================================================

#ifdef HEADERONLY
    #include "Material.cpp"
#endif
