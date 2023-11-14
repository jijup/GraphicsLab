// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once
#include <ostream>

/// @brief This class allows to discard stream output, similarly to sending output
/// to "/dev/null" while remaining portable. See apps/isoremesh for example usage
class NullStream : public std::ostream { 
private:
    class NullBuffer : public std::streambuf{
    public:
        int overflow(int c) { return c; }
    } m_sb;    
public: 
    NullStream() : std::ostream(&m_sb) {} 
};
