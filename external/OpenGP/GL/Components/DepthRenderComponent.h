// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <OpenGP/GL/Entity.h>
#include <OpenGP/GL/SyntheticDepthmap.h>
#include <OpenGP/GL/Components/TransformComponent.h>


//=============================================================================
namespace OpenGP {
//=============================================================================

/// A component representing some geometry that may be rendered to a depthmap
class DepthRenderComponent : public Component {
private:

    std::unique_ptr<DepthmapRenderer> renderer;

public:

    /// Should the object be rendered
    bool visible = true;

    void init() {
        require<TransformComponent>();
    }

    /// Construct a new renderer of type `T` with the given arguments
    template <typename T, typename ...Args>
    T &set_renderer(Args& ...args) {
        T *t = new T(args...);
        renderer = std::unique_ptr<DepthmapRenderer>(t);
        return *t;
    }

    /// Get a reference to the renderer
    template <typename T = DepthmapRenderer>
    T &get_renderer() {
        /// The contained renderer must exist and be convertible to type `T&`
        assert(renderer);
        return *dynamic_cast<T*>(renderer.get());
    }

};

//=============================================================================
} // OpenGP::
//=============================================================================
