// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "Entity.h"


//=============================================================================
namespace OpenGP {
//=============================================================================

void Entity::update() {

    for (auto &pair : components) {

        pair.second->update();

    }

}

Scene &Entity::get_scene() {
    assert(scene != nullptr);
    return *scene;
}

//=============================================================================
} // OpenGP::
//=============================================================================
