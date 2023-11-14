// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once
#include <QGLWidget>
#include <OpenGP/GL/QGLFormat32.h>

//== NAMESPACE ================================================================
namespace OpenGP {
//=============================================================================

class QGLWidget32 : public QGLWidget{ 
public:
    QGLWidget32(QWidget* parent=0) : 
        QGLWidget(QGLFormat32(), parent){}
};

//=============================================================================
} // namespace OpenGP
//=============================================================================
