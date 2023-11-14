// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once
#include <QGLFormat>

//== NAMESPACE ================================================================
namespace OpenGP {
//=============================================================================

class QGLFormat32 : public QGLFormat {
public:
    QGLFormat32() {
        setVersion(3,2);
        setProfile(QGLFormat::CoreProfile);
        setSampleBuffers(true); ///< anti-aliasing
    }
};

//=============================================================================
} // namespace OpenGP
//=============================================================================
