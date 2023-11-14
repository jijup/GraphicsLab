// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

///--- Load OpenGL here (GLEW is for cross platform)
#include <GL/glew.h> //< must be before glfw

///--- Linux needs extensions for framebuffers
#if __unix__
    #define GL_GLEXT_PROTOTYPES 1
    #include <GL/gl.h>
    #include <GL/glext.h>
#endif
