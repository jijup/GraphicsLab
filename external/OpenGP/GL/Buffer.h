// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once
#include <OpenGP/GL/gl.h>
#include <OpenGP/GL/gl_types.h>
#include <OpenGP/util/math_types.h>
#include <vector>

//=============================================================================
namespace OpenGP {
//=============================================================================

template <GLenum TARGET>
class GenericBuffer {
protected:

    GLuint buffer = 0;     ///< 0: invalid

public:

    static constexpr GLenum target = TARGET;

    GenericBuffer() {
        glGenBuffers(1, &buffer);
    }

    virtual ~GenericBuffer(){ glDeleteBuffers(1, &buffer); }

    void bind() { glBindBuffer(TARGET, buffer); }
    void unbind() { glBindBuffer(TARGET, 0); }

    GLuint id() const { return buffer; }

    /// @note use the other upload functions whenever possible
    virtual void upload_raw_block(const GLvoid* raw_data_ptr, GLsizeiptr block_size, GLenum usage=GL_STATIC_DRAW){
        glBindBuffer(TARGET, buffer);
        glBufferData(TARGET, block_size, raw_data_ptr, usage);
    }

};

template <GLenum TARGET>
class VectorBuffer : public GenericBuffer<TARGET> {
protected:

    GLsizeiptr num_elems = 0; ///< # of uploaded elements

public:

    VectorBuffer() = default;

    virtual ~VectorBuffer() {}

    GLsizeiptr size() const { return num_elems; }
    virtual GLsizeiptr elem_size() const = 0;
    virtual GLenum get_data_type() const = 0;
    virtual GLuint get_components() const = 0;

    /// @note use the other upload functions whenever possible
    void upload_raw_block(const GLvoid* raw_data_ptr, GLsizeiptr block_size, GLenum usage=GL_STATIC_DRAW){
        num_elems = block_size / elem_size();
        glBindBuffer(TARGET, this->buffer);
        glBufferData(TARGET, block_size, raw_data_ptr, usage);
    }

};

template <GLenum TARGET, class T>
class Buffer : public VectorBuffer<TARGET> {
public:

    using Scalar = UnderlyingScalar<T>;

    Buffer() = default;

    Buffer(const Buffer&) = delete;
    Buffer &operator=(const Buffer&) = delete;

    void upload(const std::vector<T>& data, GLenum usage=GL_STATIC_DRAW){
        this->num_elems = data.size();
        upload_raw(data.data(), this->num_elems, usage);
    }

    /// @note use the other upload functions whenever possible
    void upload_raw(const GLvoid* raw_data_ptr, GLsizeiptr num_elems, GLenum usage=GL_STATIC_DRAW){
        this->num_elems = num_elems;
        glBindBuffer(TARGET, this->buffer);
        glBufferData(TARGET, this->num_elems * sizeof(T), raw_data_ptr, usage);
    }

    GLsizeiptr elem_size() const { return sizeof(T); }
    GLenum get_data_type() const { return GLType<Scalar>(); }
    GLuint get_components() const { return ScalarComponents<T>(); }

};

///--- Specializations

using GenericArrayBuffer = GenericBuffer<GL_ARRAY_BUFFER>;

using GenericElementArrayBuffer = GenericBuffer<GL_ELEMENT_ARRAY_BUFFER>;

using VectorArrayBuffer = VectorBuffer<GL_ARRAY_BUFFER>;

using VectorElementArrayBuffer = VectorBuffer<GL_ELEMENT_ARRAY_BUFFER>;

template <class T>
using ArrayBuffer = Buffer<GL_ARRAY_BUFFER, T>;

template <class T>
using ElementArrayBuffer = Buffer<GL_ELEMENT_ARRAY_BUFFER, T>;

//=============================================================================
} // namespace OpenGP
//=============================================================================
