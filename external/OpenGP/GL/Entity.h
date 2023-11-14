// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <unordered_map>
#include <memory>
#include <typeindex>
#include <cassert>

#include <OpenGP/headeronly.h>


//=============================================================================
namespace OpenGP {
//=============================================================================

class Scene;
class Entity;

/// The base class for all components
class Component {

    friend class Entity;

private:

    Entity *entity = nullptr;
    Scene *scene = nullptr;

protected:

    Component() {}

public:

    Component(const Component&) = delete;
    Component &operator=(const Component&) = delete;

    /// @brief Overridable method that handles object initialization
    /// @warning any initialization code that depends on the state of the entity, scene, or other components should go here, **not** in the constructor
    virtual void init() {}

    /// Overridable method that handles per-frame updates
    virtual void update() {}

    /// Get a reference to the entity that holds this component
    Entity &get_entity() { assert(entity != nullptr); return *entity; }
    /// Get a reference to the entity that holds this component
    const Entity &get_entity() const { assert(entity != nullptr); return *entity; }

    /// Get a reference to the scene that contains the parent entity
    Scene &get_scene() { assert(scene != nullptr); return *scene; }
    /// Get a reference to the scene that contains the parent entity
    const Scene &get_scene() const { assert(scene != nullptr); return *scene; }

    /// Add the specified component to the parent entity if it does not already exist
    template <typename T>
    T &require();

    /// Check if the parent entity has the specified component
    template <typename T>
    bool has();

    /// Get a reference to the specified component in the parent entity
    template <typename T>
    T &get();

    /// Get a reference to the specified component in the parent entity
    template <typename T>
    const T &get() const;

};

/// A basic container for components that describes one node in the scene graph
class Entity {

    friend class Scene;

private:

    Scene *scene = nullptr;

    std::unordered_map<std::type_index, std::unique_ptr<Component>> components;

public:

    Entity() {}

    /// Execute the per-frame update step for this entity's components
    HEADERONLY_INLINE void update();

    Entity(const Entity&) = delete;
    Entity &operator=(const Entity&) = delete;

    /// Add the specified component to this entity if it does not already exist
    template <typename T>
    T &require() {
        assert(scene != nullptr);

        if (has<T>())
            return get<T>();

        T *component = new T();
        components[std::type_index(typeid(T))] = std::unique_ptr<Component>(component);

        component->entity = this;
        component->scene = scene;
        component->init();

        return *component;
    }

    /// Check if this entity has the specified component
    template <typename T>
    bool has() const {
        return components.find(std::type_index(typeid(T))) != components.end();
    }

    /// Get a reference to the specified component in this entity
    template <typename T>
    const T &get() const {
        return dynamic_cast<const T&>(*(components.at(std::type_index(typeid(T)))));
    }

    /// Get a reference to the specified component in this entity
    template <typename T>
    T &get() {
        return dynamic_cast<T&>(*(components.at(std::type_index(typeid(T)))));
    }

    /// Get a reference to the scene that contains this entity
    HEADERONLY_INLINE Scene &get_scene();

};

template <typename T>
T &Component::require() { return get_entity().require<T>(); }

template <typename T>
bool Component::has() { return get_entity().has<T>(); }

template <typename T>
T &Component::get() { return get_entity().get<T>(); }

template <typename T>
const T &Component::get() const { return get_entity().get<T>(); }

//=============================================================================
} // OpenGP::
//=============================================================================

#ifdef HEADERONLY
    #include "Entity.cpp"
#endif
