// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once
#include <OpenGP/GL/gl.h>

//=============================================================================
namespace OpenGP {
//=============================================================================

class VertexArrayObject{
    GLuint VAO;
public:

    VertexArrayObject(){
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
    }

    VertexArrayObject(const VertexArrayObject&) = delete;
    VertexArrayObject &operator=(const VertexArrayObject&) = delete;

    ~VertexArrayObject(){ glDeleteVertexArrays(1, &VAO); }

    void bind(){ glBindVertexArray(VAO); }

#if __cplusplus >= 201402L
    [[deprecated]]
#endif
    void release(){ glBindVertexArray(0); }

    void unbind(){ glBindVertexArray(0); }

};

//=============================================================================
} // namespace OpenGP
//=============================================================================
