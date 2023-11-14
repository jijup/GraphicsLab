// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

/// HEADERONLY_INLINE is either nothing or the keyword "inline"
#ifndef OPENGP_HEADERONLY
    #undef HEADERONLY
    #undef HEADERONLY_INLINE
    #define HEADERONLY_INLINE
#else
    #undef HEADERONLY
    #define HEADERONLY
    #undef HEADERONLY_INLINE
    #define HEADERONLY_INLINE inline
#endif
