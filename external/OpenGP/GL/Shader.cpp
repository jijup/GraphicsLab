// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <OpenGP/GL/Shader.h>

//=============================================================================
namespace OpenGP {
//=============================================================================

void Shader::clear() {
    glDeleteProgram(pid);
    _is_valid = false;
    pid = glCreateProgram();
}

void Shader::set_uniform(const char* name, int scalar) {
    assert( check_is_current() );
    if (!has_uniform(name)) return;
    GLint loc = uniforms.at(std::string(name));
    glUniform1i(loc, scalar);
}

void Shader::set_uniform(const char* name, float scalar) {
    assert( check_is_current() );
    if (!has_uniform(name)) return;
    GLint loc = uniforms.at(std::string(name));
    glUniform1f(loc, scalar);
}

void Shader::set_uniform(const char* name, const Eigen::Vector3f& vector) {
    assert( check_is_current() );
    if (!has_uniform(name)) return;
    GLint loc = uniforms.at(std::string(name));
    glUniform3fv(loc, 1, vector.data());
}

void Shader::set_uniform(const char* name, const Eigen::Matrix4f& matrix) {
    assert( check_is_current() );
    if (!has_uniform(name)) return;
    GLint loc = uniforms.at(std::string(name));
    glUniformMatrix4fv(loc, 1, GL_FALSE, matrix.data());
}

void Shader::get_uniform(const char* name, int &scalar) {
    assert( check_is_current() );
    GLint loc = uniforms.at(std::string(name));
    glGetUniformiv(pid, loc, &scalar);
}

void Shader::get_uniform(const char* name, float &scalar) {
    assert( check_is_current() );
    GLint loc = uniforms.at(std::string(name));
    glGetUniformfv(pid, loc, &scalar);
}

void Shader::get_uniform(const char* name, Eigen::Vector3f& vector) {
    assert( check_is_current() );
    GLint loc = uniforms.at(std::string(name));
    glGetUniformfv(pid, loc, vector.data());
}

void Shader::get_uniform(const char* name, Eigen::Matrix4f& matrix) {
    assert( check_is_current() );
    GLint loc = uniforms.at(std::string(name));
    glGetUniformfv(pid, loc, matrix.data());
}

bool OpenGP::Shader::has_uniform(const char* name) const {
    return uniforms.find(std::string(name)) != uniforms.end();
}

void Shader::set_attribute(const char* name, float value) {
    assert( check_is_current() );
    if (!has_attribute(name)) return;
    GLint loc = attributes.at(std::string(name));
    glVertexAttrib1f(loc, value);
}

void OpenGP::Shader::set_attribute(const char* name, const Eigen::Vector3f& vector) {
    assert( check_is_current() );
    if (!has_attribute(name)) return;
    GLint loc = attributes.at(std::string(name));
    glVertexAttrib3fv(loc, vector.data());
}

void OpenGP::Shader::set_attribute(const char* name, ArrayBuffer<float>& buffer, GLuint divisor) {
    assert( check_is_current() );
    if (!has_attribute(name)) return;
    GLint location = attributes.at(std::string(name));
    glEnableVertexAttribArray(location); ///< cached in VAO
    buffer.bind(); ///< memory the description below refers to
    glVertexAttribPointer(location, /*vec1*/ 1, GL_FLOAT, DONT_NORMALIZE, ZERO_STRIDE, ZERO_BUFFER_OFFSET);
    glVertexAttribDivisor(location, divisor);
}

void OpenGP::Shader::set_attribute(const char* name, ArrayBuffer<Eigen::Vector2f>& buffer, GLuint divisor) {
    assert( check_is_current() );
    if (!has_attribute(name)) return;
    GLint location = attributes.at(std::string(name));
    glEnableVertexAttribArray(location); ///< cached in VAO
    buffer.bind(); ///< memory the description below refers to
    glVertexAttribPointer(location, /*vec3*/ 2, GL_FLOAT, DONT_NORMALIZE, ZERO_STRIDE, ZERO_BUFFER_OFFSET);
    glVertexAttribDivisor(location, divisor);
}

void OpenGP::Shader::set_attribute(const char* name, ArrayBuffer<Eigen::Vector3f>& buffer, GLuint divisor) {
    assert( check_is_current() );
    if (!has_attribute(name)) return;
    GLint location = attributes.at(std::string(name));
    glEnableVertexAttribArray(location); ///< cached in VAO
    buffer.bind(); ///< memory the description below refers to
    glVertexAttribPointer(location, /*vec3*/ 3, GL_FLOAT, DONT_NORMALIZE, ZERO_STRIDE, ZERO_BUFFER_OFFSET);
    glVertexAttribDivisor(location, divisor);
}

void OpenGP::Shader::set_attribute(const char* name, VectorArrayBuffer &buffer, GLuint divisor) {
    assert( check_is_current() );
    if (!has_attribute(name)) return;
    GLint location = attributes.at(std::string(name));
    glEnableVertexAttribArray(location); ///< cached in VAO
    buffer.bind(); ///< memory the description below refers to
    glVertexAttribPointer(location, buffer.get_components(), buffer.get_data_type(), DONT_NORMALIZE, ZERO_STRIDE, ZERO_BUFFER_OFFSET);
    glVertexAttribDivisor(location, divisor);
}

bool OpenGP::Shader::has_attribute(const char* name) const {
    return attributes.find(std::string(name)) != attributes.end();
}

bool OpenGP::Shader::add_vshader_from_source(const char* code) {
    if (verbose) mDebug() << "Compiling Vertex shader";

    /// Create the Vertex Shader
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);

    /// Compile Vertex Shader
    glShaderSource(VertexShaderID, 1, &code, NULL);
    glCompileShader(VertexShaderID);

    /// Check Vertex Shader
    GLint Success = GL_FALSE;
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Success);
    if (!Success) {
        int InfoLogLength;
        glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        std::vector<char> VertexShaderErrorMessage(InfoLogLength);
        glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        // fprintf(stdout, "Failed:\n%s\n", &VertexShaderErrorMessage[0]);
        mDebug() << std::string(&VertexShaderErrorMessage[0]);
    } else {
        glAttachShader(pid, VertexShaderID);
    }

    // Make sure shaders get garbage collected
    glDeleteShader(VertexShaderID);

    return Success;
}

bool OpenGP::Shader::add_fshader_from_source(const char* code) {
    if (verbose) mDebug() << "Compiling Fragment shader";

    /// Create the Fragment Shader
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    /// Compile Fragment Shader
    glShaderSource(FragmentShaderID, 1, &code, NULL);
    glCompileShader(FragmentShaderID);

    /// Check Fragment Shader
    GLint Success = GL_FALSE;
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Success);
    if (!Success) {
        int InfoLogLength;
        glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        mDebug() << std::string(&FragmentShaderErrorMessage[0]);
    } else {
        glAttachShader(pid, FragmentShaderID);
    }

    // Make sure shaders get garbage collected
    glDeleteShader(FragmentShaderID);

    return Success;
}

bool OpenGP::Shader::add_shader_from_source(const char* code, GLenum type) {
    if (verbose) mDebug() << "Compiling shader";

    /// Create the Shader
    GLuint ShaderID = glCreateShader(type);

    /// Compile Shader
    glShaderSource(ShaderID, 1, &code, NULL);
    glCompileShader(ShaderID);

    /// Check Shader
    GLint Success = GL_FALSE;
    glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &Success);
    if (!Success) {
        int InfoLogLength;
        glGetShaderiv(ShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        std::vector<char> ShaderErrorMessage(InfoLogLength);
        glGetShaderInfoLog(ShaderID, InfoLogLength, NULL, &ShaderErrorMessage[0]);
        mDebug() << std::string(&ShaderErrorMessage[0]);
    } else {
        glAttachShader(pid, ShaderID);
    }

    // Make sure shaders get garbage collected
    glDeleteShader(ShaderID);

    return Success;
}

bool OpenGP::Shader::link() {
    if (verbose) mDebug() << "Linking shader program";
    glLinkProgram(pid);

    /// Check the program
    GLint Success = GL_FALSE;
    glGetProgramiv(pid, GL_LINK_STATUS, &Success);
    if (!Success) {
        int InfoLogLength;
        glGetProgramiv(pid, GL_INFO_LOG_LENGTH, &InfoLogLength);
        std::vector<char> ProgramErrorMessage( std::max(InfoLogLength, int(1)) );
        glGetProgramInfoLog(pid, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        mDebug() << "Failed: " << &ProgramErrorMessage[0];
    } else {

        uniforms.clear();
        attributes.clear();

        _is_valid = true;

        GLchar buffer[128];

        GLint attribs_count, attrib_size, attrib_location;
        GLenum attrib_type;
        glGetProgramiv(pid, GL_ACTIVE_ATTRIBUTES, &attribs_count);
        for (GLint i = 0;i < attribs_count;i++) {
            glGetActiveAttrib(pid, i, 128, nullptr, &attrib_size, &attrib_type, buffer);
            attrib_location = glGetAttribLocation(pid, buffer);
            attributes[std::string(buffer)] = attrib_location;
        }

        GLint uniforms_count, uniform_size, uniform_location;
        GLenum uniform_type;
        glGetProgramiv(pid, GL_ACTIVE_UNIFORMS, &uniforms_count);
        for (GLint i = 0;i < uniforms_count;i++) {
            glGetActiveUniform(pid, i, 128, nullptr, &uniform_size, &uniform_type, buffer);
            uniform_location = glGetUniformLocation(pid, buffer);
            uniforms[std::string(buffer)] = uniform_location;
        }

    }

    return Success;
}

//=============================================================================
} // namespace OpenGP
//=============================================================================
