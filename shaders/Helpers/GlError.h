// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once
#include<iostream>
#include <GL/glew.h>

static inline const char* ErrorString(GLenum error) {
  const char* msg;
  switch (error) {
#define Case(Token)  case Token: msg = #Token; break;
  Case(GL_INVALID_ENUM);
  Case(GL_INVALID_VALUE);
  Case(GL_INVALID_OPERATION);
  Case(GL_INVALID_FRAMEBUFFER_OPERATION);
  Case(GL_NO_ERROR);
  Case(GL_OUT_OF_MEMORY);
#undef Case
  }

  return msg;
}

static inline void _glCheckError(const char* file, int line) {
  GLenum error;
  while ((error = glGetError()) != GL_NO_ERROR) {
    fprintf(stderr, "ERROR: file %s, line %i: %s.\n", file, line,
            ErrorString(error));
  }
}

#ifndef NDEBUG
    #define check_error_gl() _glCheckError(__FILE__, __LINE__)
#else
    #define check_error_gl() ((void)0)
#endif
