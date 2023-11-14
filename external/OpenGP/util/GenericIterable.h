// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <functional>
#include <type_traits>


//=============================================================================
namespace OpenGP {
//=============================================================================

template <typename T>
class GenericIterable {
public:

    using value_type = typename std::remove_reference<T>::type;

    using Advancer = std::function<value_type*()>;

    class Iterator {
    private:

        value_type *ptr;

        Advancer advancer;

    public:

        Iterator(Advancer advancer) {
            this->advancer = advancer;
            ptr = this->advancer();
        }

        value_type &operator*() { return *ptr; }

        bool operator==(const Iterator &rhs) const {
            return ptr == rhs.ptr;
        }

        bool operator!=(const Iterator &rhs) const {
            return ptr != rhs.ptr;
        }

        Iterator &operator++() {
            ptr = advancer();
            return *this;
        }

    };

private:

    Advancer base_advancer;

public:

    GenericIterable(Advancer advancer) {
        base_advancer = advancer;
    }

    Iterator begin() {
        return Iterator(base_advancer);
    }

    Iterator end() {
        return Iterator([](){ return nullptr; });
    }

    template <typename I>
    static GenericIterable adaptor(I &iterable) {

        auto it = iterable.begin();
        auto end = iterable.end();

        Advancer advancer = [it, end]() mutable {
            if (it != end) {
                T *ptr = &(*it);
                ++it;
                return ptr;
            }
            return (T*)nullptr;
        };

        return GenericIterable(advancer);

    }

    GenericIterable filter(std::function<bool(T&)> predicate) {
        Advancer advancer = base_advancer;
        GenericIterable new_iterable([predicate, advancer](){

            T *ptr = nullptr;

            do {
                ptr = advancer();
            } while(ptr != nullptr && !predicate(*ptr));

            return ptr;

        });
        return new_iterable;
    }


    /// Perform a map transformation @note predicate must return a pointer
    template <typename Predicate>
    GenericIterable<typename std::remove_reference<typename std::remove_pointer<typename std::result_of<Predicate(T&)>::type>::type>::type> map(Predicate predicate) {

        using ResultValueType = typename std::remove_reference<typename std::remove_pointer<typename std::result_of<Predicate(T&)>::type>::type>::type;

        Advancer advancer = base_advancer;
        GenericIterable<ResultValueType> new_iterable([predicate, advancer](){
            value_type *ptr = advancer();
            if (ptr == nullptr) {
                return (ResultValueType*)nullptr;
            } else {
                return predicate(*ptr);
            }
        });
        return new_iterable;
    }

    template <typename U>
    GenericIterable<U> cast() {
        Advancer advancer = base_advancer;
        GenericIterable new_iterable([advancer](){
            return dynamic_cast<U*>(advancer());
        });
        return new_iterable;
    }

};

//=============================================================================
} // OpenGP::
//=============================================================================
