// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <type_traits>

#include <OpenGP/types.h>
#include <OpenGP/GL/gl.h>

//=============================================================================
namespace OpenGP {
//=============================================================================

template <typename Scalar, typename=void>
struct GLTypeHelper {
    static_assert(sizeof(Scalar)!=sizeof(Scalar), "No corresponding OpenGL type");
};

template <>
struct GLTypeHelper<float> {
    static constexpr GLenum type = GL_FLOAT;
};

template <typename Scalar>
struct GLTypeHelper<Scalar, typename std::enable_if<
    std::is_integral<Scalar>::value &&
    std::is_unsigned<Scalar>::value &&
    sizeof(Scalar) == 1>::type> {
    static constexpr GLenum type = GL_UNSIGNED_BYTE;
};

template <typename Scalar>
struct GLTypeHelper<Scalar, typename std::enable_if<
    std::is_integral<Scalar>::value &&
    std::is_signed<Scalar>::value &&
    sizeof(Scalar) == 1>::type> {
    static constexpr GLenum type = GL_BYTE;
};

template <typename Scalar>
struct GLTypeHelper<Scalar, typename std::enable_if<
    std::is_integral<Scalar>::value &&
    std::is_unsigned<Scalar>::value &&
    sizeof(Scalar) == 2>::type> {
    static constexpr GLenum type = GL_UNSIGNED_SHORT;
};

template <typename Scalar>
struct GLTypeHelper<Scalar, typename std::enable_if<
    std::is_integral<Scalar>::value &&
    std::is_signed<Scalar>::value &&
    sizeof(Scalar) == 2>::type> {
    static constexpr GLenum type = GL_SHORT;
};

template <typename Scalar>
struct GLTypeHelper<Scalar, typename std::enable_if<
    std::is_integral<Scalar>::value &&
    std::is_unsigned<Scalar>::value &&
    sizeof(Scalar) == 4>::type> {
    static constexpr GLenum type = GL_UNSIGNED_INT;
};

template <typename Scalar>
struct GLTypeHelper<Scalar, typename std::enable_if<
    std::is_integral<Scalar>::value &&
    std::is_signed<Scalar>::value &&
    sizeof(Scalar) == 4>::type> {
    static constexpr GLenum type = GL_INT;
};

template <typename Scalar>
constexpr GLenum GLType() {
    return GLTypeHelper<Scalar>::type;
}

//=============================================================================
} // namespace OpenGP
//=============================================================================
