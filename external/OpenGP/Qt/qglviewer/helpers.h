// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once
#include <OpenGP/types.h>
#include <QGLViewer/vec.h>

namespace qglviewer{
    Vec tr(OpenGP::Vec3 vec){ return Vec(vec.x(), vec.y(), vec.z()); }
} // qglviewer::
