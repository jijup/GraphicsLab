// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <list>

#include <OpenGP/headeronly.h>
#include <OpenGP/util/GenericIterable.h>
#include <OpenGP/GL/Entity.h>


//=============================================================================
namespace OpenGP {
//=============================================================================

/// A scene graph populated with `Entity` objects
class Scene {
private:

    std::list<Entity> entities;

public:

    /// Create a new empty scene
    Scene() {}

    Scene(const Scene&) = delete;
    Scene(Scene&&) = delete;
    Scene &operator=(const Scene&) = delete;
    Scene &operator=(Scene&&) = delete;

    /// Create a new entity within the scene
    HEADERONLY_INLINE Entity &create_entity();

    /// Create a new entity within the scene that has component `T` and any of `T`'s dependencies
    template <typename T>
    T &create_entity_with() {
        auto &entity = create_entity();
        T &t = entity.require<T>();
        return t;
    }

    /// Get an iterable object that contains all objects with the specified component
    template <typename T>
    GenericIterable<T> all_of_type() {

        auto filter_pred = [](Entity &e){
            return e.has<T>();
        };

        auto map_pred = [](Entity &e){
            return &(e.get<T>());
        };

        return GenericIterable<Entity>::adaptor(entities).filter(filter_pred).map(map_pred);

    }

    /// Run one step of the scene simulation
    HEADERONLY_INLINE void update();

};

//=============================================================================
} // OpenGP::
//=============================================================================

#ifdef HEADERONLY
    #include "Scene.cpp"
#endif
