// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once
namespace opengl{
    inline bool is_depth_test_enabled(){
        GLboolean val = 0;
        glGetBooleanv(GL_DEPTH_TEST,&val);
        return val;        
    }
}