// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <type_traits>

#include <OpenGP/types.h>

//=============================================================================
namespace OpenGP {
//=============================================================================

template <typename T, typename=void>
struct UnderlyingScalarHelper {
    using type = typename UnderlyingScalarHelper<typename T::Scalar>::type;
};

template <typename T>
struct UnderlyingScalarHelper<T, typename std::enable_if<std::is_arithmetic<T>::value>::type> {
    using type = T;
};

template <typename T>
using UnderlyingScalar = typename UnderlyingScalarHelper<T>::type;

template <typename T, typename=void>
struct ScalarComponentsHelper {
    static constexpr int value = T::RowsAtCompileTime * T::ColsAtCompileTime * ScalarComponentsHelper<typename T::Scalar>::value;
};

template <typename T>
struct ScalarComponentsHelper<T, typename std::enable_if<std::is_arithmetic<T>::value>::type> {
    static constexpr int value = 1;
};

template <typename T>
constexpr int ScalarComponents() {
    return ScalarComponentsHelper<T>::value;
}

//=============================================================================
} // namespace OpenGP
//=============================================================================
