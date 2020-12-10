#ifndef CONTACTLISTENER_HPP_GUARD
#define CONTACTLISTENER_HPP_GUARD

#include "Box2D/Box2D.h"
#include "Enemy.hpp"
#include "Shot.hpp"

class MyContactListener : public b2ContactListener {
    void BeginContact(b2Contact* contact)
    {
        void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
        if (bodyUserData)
            static_cast<Shot*>(bodyUserData)->collide();

        bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
        if (bodyUserData)
            static_cast<Shot*>(bodyUserData)->collide();
    }

    void EndContact(b2Contact* /*contact*/) { }
};

#endif
