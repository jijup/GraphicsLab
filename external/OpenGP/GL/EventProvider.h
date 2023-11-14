// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <unordered_map>
#include <memory>
#include <typeindex>
#include <list>
#include <iostream>


//=============================================================================
namespace OpenGP {
//=============================================================================

class EventProvider;

/// @brief Object that allows callbacks to be automatically deregistered if the
/// objects they reference are destroyed @sa `EventProvider`
class EventSentinel {

    friend class EventProvider;

private:

    std::shared_ptr<int> handle;

public:

    ~EventSentinel() {}

    EventSentinel() : handle(new int(0)) {}

    EventSentinel(const EventSentinel&) = delete;
    EventSentinel(EventSentinel&&) = default;

    EventSentinel &operator=(const EventSentinel&) = delete;
    EventSentinel &operator=(EventSentinel&&) = default;

    /// Allow all guarded callbacks to be destroyed, as if the sentinel had died
    void reset() {
        handle = std::shared_ptr<int>(new int(0));
    }

};

///  @brief Base class that provides derived classes the abillity to register
/// callbacks and send events
class EventProvider {
private:

    std::unordered_map<std::type_index, std::list<std::function<void(void*)>>> listeners;

    EventSentinel own_sentinel;

protected:

    /// @brief A derived class can call this method to broadcast an event to all
    /// of its registered listeners
    template <typename EventType>
    void send_event(const EventType &event) {

        auto idx = std::type_index(typeid(EventType));

        for (auto &wrapper : listeners[idx]) {
            wrapper(reinterpret_cast<void*>(const_cast<EventType*>(&event)));
        }

    }

public:

    /// A handle that can be use to deregister a particular listener
    using ListenerHandle = decltype(listeners)::mapped_type::iterator;

    /// Register a listener to be called for events of `EventType`
    template <typename EventType, typename ListenerType>
    ListenerHandle add_listener(ListenerType listener) {
        return add_listener<EventType>(listener, own_sentinel);
    }

    /// Register a listener to be called for events of `EventType`
    template <typename EventType, typename ListenerType>
    ListenerHandle add_listener(ListenerType listener, const EventSentinel &sentinel) {

        /// If `sentinel` dies before the event provider, the listener will be deregistered

        auto idx = std::type_index(typeid(EventType));

        auto &llist = listeners[idx];
        auto it = llist.insert(llist.end(), std::function<void(void*)>());

        std::weak_ptr<void> handle(sentinel.handle);

        *it = [this, listener, it, handle](void *ptr){

            if (auto lock = handle.lock()) {

                listener(*reinterpret_cast<const EventType*>(ptr));

            } else {

                // TODO: fix segfault that this causes
                //remove_listener<EventType>(it);

            }

        };

        return it;

    }

    /// Manually deregister a particular listener
    template <typename EventType>
    void remove_listener(const ListenerHandle &handle) {

        auto idx = std::type_index(typeid(EventType));

        listeners[idx].erase(handle);

    }

};

//=============================================================================
} // OpenGP::
//=============================================================================
