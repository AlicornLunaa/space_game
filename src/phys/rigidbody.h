/**
 * @file rigidbody.h
 * @author Luna
 * @brief A body to have physics calculations applied to
 */
#include <SFML/System.hpp>
#include "collider.h"

namespace Phys {

    class RigidBody : public Collider {
    private:
        sf::Vector2f velocity;
        sf::Vector2f acceleration;

    public:
        RigidBody();
        ~RigidBody();
    };

};