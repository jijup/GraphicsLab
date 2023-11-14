// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <iostream>

#include <OpenGP/GL/gl.h>


//=============================================================================
namespace OpenGP {
//=============================================================================

namespace GLDebug {

    inline const char *gl_debug_source_str(GLenum source) {

        switch(source) {
            case GL_DEBUG_SOURCE_API:             return "API";
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   return "Window System";
            case GL_DEBUG_SOURCE_SHADER_COMPILER: return "Shader Compiler";
            case GL_DEBUG_SOURCE_THIRD_PARTY:     return "Third Party";
            case GL_DEBUG_SOURCE_APPLICATION:     return "Application";
            case GL_DEBUG_SOURCE_OTHER:           return "Other";
        }

        return "Unknown";

    }

    inline const char *gl_debug_type_str(GLenum type) {

        switch(type) {
            case GL_DEBUG_TYPE_ERROR:               return "Error";
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "Deprecated Behavior";
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  return "Undefined Behavior";
            case GL_DEBUG_TYPE_PORTABILITY:         return "Portability";
            case GL_DEBUG_TYPE_PERFORMANCE:         return "Performance";
            case GL_DEBUG_TYPE_MARKER:              return "Marker";
            case GL_DEBUG_TYPE_PUSH_GROUP:          return "Push Group";
            case GL_DEBUG_TYPE_POP_GROUP:           return "Pop Group";
        }

        return "Unknown";

    }

    inline const char *gl_debug_severity_str(GLenum severity) {

        switch(severity) {
            case GL_DEBUG_SEVERITY_HIGH:         return "High";
            case GL_DEBUG_SEVERITY_MEDIUM:       return "Medium";
            case GL_DEBUG_SEVERITY_LOW:          return "Low";
            case GL_DEBUG_SEVERITY_NOTIFICATION: return "Notification";
        }

        return "Unknown";

    }

    inline void gl_debug_handler(GLenum source, GLenum type, GLuint /*id*/, GLenum severity, GLsizei /*length*/, const GLchar *msg, void*) {

        const char *color;
        switch(severity) {
            case GL_DEBUG_SEVERITY_HIGH:         color = "\033[31m"; break; /* Red */
            case GL_DEBUG_SEVERITY_MEDIUM:       color = "\033[33m"; break; /* Yellow */
            case GL_DEBUG_SEVERITY_LOW:          color = "\033[36m"; break; /* Cyan */
            case GL_DEBUG_SEVERITY_NOTIFICATION: color = "\033[0m";  break; /* Normal */
        }

    #ifndef _WIN32
        std::cout << color;
    #endif
        std::cout << std::endl;

        std::cout << "-- OpenGL Debug Message --" << std::endl;
        std::cout << "Severity: " << gl_debug_severity_str(severity) << std::endl;
        std::cout << "Source:   " << gl_debug_source_str(source) << std::endl;
        std::cout << "Type:     " << gl_debug_type_str(type) << std::endl;
        std::cout << "Message:" << std::endl << " > " << msg << std::endl;

    #ifndef _WIN32
        std::cout << "\033[0m";
    #endif
        std::cout << std::endl;

    }

    inline void enable(bool show_notifications=false) {

        if (GLEW_KHR_debug) {

            glEnable(GL_DEBUG_OUTPUT);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
            if (!show_notifications)
                glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);

            glDebugMessageCallback((GLDEBUGPROC)gl_debug_handler, nullptr);

        }

    }

}

//=============================================================================
} // OpenGP::
//=============================================================================
