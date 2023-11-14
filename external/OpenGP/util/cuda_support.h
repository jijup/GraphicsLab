// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once


#ifdef __CUDACC__
    #define OPENGP_DEVICE_FUNC __host__ __device__
#else
    #define OPENGP_DEVICE_FUNC
#endif

#ifndef __CUDA_ARCH__
    #include <cmath>
    #define MATH_STD std
#else
    #define MATH_STD
#endif


//=============================================================================
namespace OpenGP {
//=============================================================================

namespace cuda_support {

    template <typename T>
    OPENGP_DEVICE_FUNC bool isfinite(const T& a) {
        return MATH_STD::isfinite(a);
    }

}

//=============================================================================
} // OpenGP::
//=============================================================================

#undef MATH_STD
