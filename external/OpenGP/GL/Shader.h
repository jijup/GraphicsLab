// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <string>
#include <unordered_map>

#include <Eigen/Dense>
#include <OpenGP/GL/gl.h>
#include <OpenGP/headeronly.h>
#include <OpenGP/MLogger.h>
#include <OpenGP/GL/Buffer.h>

//=============================================================================
namespace OpenGP {
//=============================================================================

namespace{
    /// Convenience constants
    static const int ONE = 1;
    static const bool DONT_NORMALIZE = false;
    static const bool DONT_TRANSPOSE = false;
    static const int ZERO_STRIDE = 0;
    static const void* ZERO_BUFFER_OFFSET = 0;
}

/// @note Inspired by QOpenGLShaderProgram
/// @note Important not to use Scalar
class Shader{
/// @{
private:
    GLuint pid = 0; ///< 0: invalid
    bool _is_valid = false;
    std::unordered_map<std::string, GLuint> attributes;
    std::unordered_map<std::string, GLuint> uniforms;
public:
    bool verbose = false; ///< prints messages
/// @}

public:

    Shader(){ pid = glCreateProgram(); }

    Shader(const Shader&) = delete;
    Shader &operator=(const Shader&) = delete;

    ~Shader() { glDeleteProgram(pid); }

    GLuint programId() const { return pid; }

    void bind(){ assert(_is_valid); glUseProgram(pid); }
    void unbind(){ glUseProgram(0); }

#if __cplusplus >= 201402L
    [[deprecated]]
#endif
    void release(){ glUseProgram(0); }

    HEADERONLY_INLINE void clear();

    bool is_valid(){ return _is_valid; }

    bool check_is_current(){
        assert(pid!=0);
        return (current_program_id() == pid);
    }

    static GLuint current_program_id(){
        GLint id;
        glGetIntegerv(GL_CURRENT_PROGRAM,&id);
        return (GLuint) id;
    }

/// @{ shaders compile and link
public:
    HEADERONLY_INLINE bool add_vshader_from_source(const char* code);
    HEADERONLY_INLINE bool add_fshader_from_source(const char* code);
    HEADERONLY_INLINE bool add_shader_from_source(const char* code, GLenum type);
    bool add_gshader_from_source(const char* code) {
        return add_shader_from_source(code, GL_GEOMETRY_SHADER);
    }
    HEADERONLY_INLINE bool link();
/// @}

/// @{ uniforms setters
public:
    HEADERONLY_INLINE void set_uniform(const char* name, int scalar);
    HEADERONLY_INLINE void set_uniform(const char* name, float scalar);
    HEADERONLY_INLINE void set_uniform(const char* name, const Eigen::Vector3f& vector);
    HEADERONLY_INLINE void set_uniform(const char* name, const Eigen::Matrix4f& matrix);
/// @}

/// @{ uniforms getters
public:
    HEADERONLY_INLINE void get_uniform(const char* name, int &scalar);
    HEADERONLY_INLINE void get_uniform(const char* name, float &scalar);
    HEADERONLY_INLINE void get_uniform(const char* name, Eigen::Vector3f& vector);
    HEADERONLY_INLINE void get_uniform(const char* name, Eigen::Matrix4f& matrix);
/// @}

    HEADERONLY_INLINE bool has_uniform(const char* name) const;

/// @{ setters for *constant* vertex attributes
public:
    HEADERONLY_INLINE void set_attribute(const char* name, float value);
    HEADERONLY_INLINE void set_attribute(const char* name, const Eigen::Vector3f& vector);
    HEADERONLY_INLINE void set_attribute(const char* name, ArrayBuffer<float>& buffer, GLuint divisor = 0);
    HEADERONLY_INLINE void set_attribute(const char* name, ArrayBuffer<Eigen::Vector2f>& buffer, GLuint divisor = 0);
    HEADERONLY_INLINE void set_attribute(const char* name, ArrayBuffer<Eigen::Vector3f>& buffer, GLuint divisor = 0);
    HEADERONLY_INLINE void set_attribute(const char* name, VectorArrayBuffer& buffer, GLuint divisor = 0);
/// @}

    HEADERONLY_INLINE bool has_attribute(const char* name) const;

};

//=============================================================================
} // namespace OpenGP
//=============================================================================

#ifdef HEADERONLY
    #include "Shader.cpp"
#endif
