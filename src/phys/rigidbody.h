/**
 * @file rigidbody.h
 * @author Luna
 * @brief A body to have physics calculations applied to
 */
#pragma once
#include <SFML/System.hpp>
#include "collider.h"

namespace Phys {

    class RigidBody : public Collider {
    public:
        sf::Vector2f velocity;
        sf::Vector2f acceleration;
        float rotVelocity;
        float rotAcceleration;
        float mass;

        RigidBody(float x, float y, float rotation);
        ~RigidBody();
    };

    
    class BoxRigidBody : public RigidBody {
    public:
        BoxRigidBody(float x, float y, float w, float h, float rotation = 0.f);
    };

};