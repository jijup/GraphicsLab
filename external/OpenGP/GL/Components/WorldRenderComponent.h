// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <OpenGP/GL/Entity.h>
#include <OpenGP/GL/MaterialRenderer.h>
#include <OpenGP/GL/Components/TransformComponent.h>


//=============================================================================
namespace OpenGP {
//=============================================================================

/// A component representing some visible object that may be rendered by a camera
class WorldRenderComponent : public Component {
private:

    std::unique_ptr<MaterialRenderer> renderer;

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
        renderer = std::unique_ptr<MaterialRenderer>(t);
        return *t;
    }

    /// Get a reference to the renderer
    template <typename T = MaterialRenderer>
    T &get_renderer() {
        /// The contained renderer must exist and be convertible to type `T&`
        assert(renderer);
        return *dynamic_cast<T*>(renderer.get());
    }

};

//=============================================================================
} // OpenGP::
//=============================================================================
