// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once
#include <OpenGP/GL/gl.h>

#include <OpenGP/GL/Texture.h>

//=============================================================================
namespace OpenGP {
//=============================================================================

class Framebuffer{
private:

    GLuint fbo;

public:

    Framebuffer() {
        glGenFramebuffers(1, &fbo);
    }

    Framebuffer(const Framebuffer&) = delete;
    Framebuffer &operator=(const Framebuffer&) = delete;

    ~Framebuffer() {
        glDeleteFramebuffers(1, &fbo);
    }

    void attach_color_texture(GenericTexture &texture, int attachment = 0) {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachment, GL_TEXTURE_2D, texture.id(), 0);
        glBindFramebuffer(GL_FRAMEBUFFER, get_default());
    }

    void attach_depth_texture(GenericTexture &texture) {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture.id(), 0);
        glBindFramebuffer(GL_FRAMEBUFFER, get_default());
    }

    void bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    }

    void unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, get_default());
    }

    static void override_default(const Framebuffer &fb) {
        get_default() = fb.fbo;
    }

    static void reset_default() {
        get_default() = 0;
    }

private:

    static GLuint &get_default() {
        static GLuint default_fbo;
        return default_fbo;
    }

};

//=============================================================================
} // namespace OpenGP
//=============================================================================
