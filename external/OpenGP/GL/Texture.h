// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <type_traits>

#include <OpenGP/GL/gl.h>
#include <OpenGP/GL/gl_types.h>
#include <OpenGP/Image/ImageType.h>

//=============================================================================
namespace OpenGP {
//=============================================================================

template <GLenum INTERNAL_FORMAT, GLenum FORMAT, GLenum TYPE>
class Texture;

template <int components>
struct ColorFormatBuilder {
    static_assert(components!=components, "Invalid texture component count");
};

template <>
struct ColorFormatBuilder<1> {
    static constexpr GLenum format = GL_RED;
};

template <>
struct ColorFormatBuilder<2> {
    static constexpr GLenum format = GL_RG;
};

template <>
struct ColorFormatBuilder<3> {
    static constexpr GLenum format = GL_RGB;
};

template <>
struct ColorFormatBuilder<4> {
    static constexpr GLenum format = GL_RGBA;
};

template <typename ImageType>
struct TextureTypeBuilder {
    using Scalar = typename ImageTypeInfo<ImageType>::Scalar;
    static constexpr int components = ImageTypeInfo<ImageType>::component_count;
    using Type = Texture<ColorFormatBuilder<components>::format, ColorFormatBuilder<components>::format, GLType<Scalar>()>;
};


class GenericTexture {
protected:

    GLuint _id;

    GLenum internal_format, format, type;

    GLuint width = -1, height = -1;

public:

    GenericTexture() {
        glGenTextures(1, &_id);
        bind();

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        unbind();
    }

    GenericTexture(const GenericTexture&) = delete;
    GenericTexture &operator=(const GenericTexture&) = delete;

    virtual ~GenericTexture() {
        glGenTextures(1, &_id);
    }

    void bind() const { glBindTexture(GL_TEXTURE_2D, _id); }
    void unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }

    void enable_filter() {
        bind();

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        unbind();
    }

    void disable_filter() {
        bind();

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        unbind();
    }

    GLuint id() const { return _id; }

    GLenum get_internal_format() const { return internal_format; }
    GLenum get_format() const { return format; }
    GLenum get_type() const { return type; }

    GLuint get_width() const {
        return width;
    }

    GLuint get_height() const {
        return height;
    }

};

template <GLenum INTERNAL_FORMAT, GLenum FORMAT, GLenum TYPE>
class Texture : public GenericTexture {
public:

    static constexpr GLenum InternalFormat = INTERNAL_FORMAT;
    static constexpr GLenum Format = FORMAT;
    static constexpr GLenum Type = TYPE;

    Texture() {
        internal_format = INTERNAL_FORMAT;
        format = FORMAT;
        type = TYPE;
    }

    void allocate(GLsizei width, GLsizei height) {

        this->width = width;
        this->height = height;

        bind();

        glTexImage2D(GL_TEXTURE_2D, 0, INTERNAL_FORMAT, width, height, 0, FORMAT, TYPE, nullptr);

        unbind();

    }

    template <typename ImageType>
    void upload(const ImageType &image) {

        using InputType = typename TextureTypeBuilder<ImageType>::Type;
        static_assert(Type == InputType::Type && Format == InputType::Format, "Incompatible Image and texture types");

        this->width = image.cols();
        this->height = image.rows();

        upload_raw(image.cols(), image.rows(), image.data());

    }

    void upload_raw(GLsizei width, GLsizei height, const void *data) {

        this->width = width;
        this->height = height;

        bind();

        glTexImage2D(GL_TEXTURE_2D, 0, INTERNAL_FORMAT, width, height, 0, FORMAT, TYPE, data);

        unbind();

    }

    template <typename ImageType>
    void download(ImageType &image) {

        //using InputType = typename TextureTypeBuilder<ImageType>::Type;
        //static_assert(Type == InputType::Type && Format == InputType::Format, "Incompatible Image and texture types");
        // TODO: find better compatibility test

        int width, height;
        bind();
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
        unbind();

        image.resize(height, width);

        download_raw(image.data());

    }

    void download_raw(void *data) {

        bind();

        glGetTexImage(GL_TEXTURE_2D, 0, FORMAT, TYPE, data);

        unbind();

    }

};

using RGBA8Texture = Texture<GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE>;
using RGB8Texture = Texture<GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE>;
using RGB32FTexture = Texture<GL_RGB32F, GL_RGB, GL_FLOAT>;
using R32FTexture = Texture<GL_R32F, GL_RED, GL_FLOAT>;
using D16Texture = Texture<GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT>;

//=============================================================================
} // namespace OpenGP
//=============================================================================
