// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "Material.h"


//=============================================================================
namespace OpenGP {
//=============================================================================

const char *Material::default_vertex_code() {
    return R"GLSL(

    void vertex_shade() {
    }

)GLSL";
}

const char *Material::default_geometry_code() {
    return R"GLSL(

    void geometry_vertex_shade(int v) {
    }

)GLSL";
}

const char *Material::default_fragment_code() {
    return R"GLSL(

        uniform vec3 base_color;

        vec4 fragment_shade() {
            vec3 lightdir = -get_forward();
            float diffuse = clamp(abs(dot(get_normal(), normalize(lightdir))), 0, 1);
            vec3 ambient = vec3(0.1,0.11,0.13);

            return vec4(diffuse * base_color + ambient, 1);
        }

    )GLSL";
}

Material::Material() : Material(default_vertex_code(), default_geometry_code(), default_fragment_code()) {
    set_property("base_color", Vec3(0.6, 0.6, 0.6));
}

Material::Material(const std::string &fragment_code) : Material(default_vertex_code(), default_geometry_code(), fragment_code) {}

Material::Material(const std::string &vertex_code, const std::string &geometry_code, const std::string &fragment_code) {
    this->vertex_code = vertex_code;
    this->geometry_code = geometry_code;
    this->fragment_code = fragment_code;
}

const std::string &Material::get_vertex_code() const {
    return vertex_code;
}
const std::string &Material::get_geometry_code() const {
    return geometry_code;
}
const std::string &Material::get_fragment_code() const {
    return fragment_code;
}

void Material::set_vertex_code(const std::string& code) {
    vertex_code = code;
}
void Material::set_geometry_code(const std::string& code) {
    geometry_code = code;
}
void Material::set_fragment_code(const std::string& code) {
    fragment_code = code;
}

void Material::apply_properties(Shader &shader) const {
    for (auto &pair : properties) {
        pair.second(shader);
    }
}

//=============================================================================
} // OpenGP::
//=============================================================================
