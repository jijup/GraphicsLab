// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <type_traits>
#include <cassert>

#include <Eigen/Dense>
#include <Eigen/Geometry>


//=============================================================================
namespace OpenGP {
//=============================================================================

template <class PixelType>
using Image = Eigen::Matrix< PixelType, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor >;

// --- Pixel type is a vector or matrix ----------------------------------------

template <typename ImageType, typename=void>
struct ImageTypeInfo {

    static constexpr int component_count = ImageType::Scalar::RowsAtCompileTime * ImageType::Scalar::ColsAtCompileTime;

    using Scalar = typename ImageType::Scalar::Scalar;

    static void set_zero(ImageType &I) {
        I = ImageType::Constant(I.rows(), I.cols(), ImageType::Scalar::Zero());
    }

    static Scalar &channel_ref(ImageType &I, int row, int col, int c) {
        assert(c < component_count && c >= 0);
        return I(row, col)(c);
    }

};

// -----------------------------------------------------------------------------


// --- Pixel type is not a vector or matrix ------------------------------------

template <typename ImageType>
struct ImageTypeInfo<ImageType, typename std::enable_if<!std::is_base_of<Eigen::EigenBase<typename ImageType::Scalar>, typename ImageType::Scalar>::value>::type> {

    static constexpr int component_count = 1;

    using Scalar = typename ImageType::Scalar;

    static void set_zero(ImageType &I) {
        I.setZero();
    }

    static Scalar &channel_ref(ImageType &I, int row, int col, int c) {
        assert(c == 0);
        return I(row, col);
    }

};

// -----------------------------------------------------------------------------

//=============================================================================
} // namespace OpenGP
//=============================================================================
