/**
 * @file rigidbody.h
 * @author Luna
 * @brief A body to have physics calculations applied to
 */
#pragma once
#include <SFML/System.hpp>
#include "colliders.h"

namespace Phys {
    
    class CollisionBody : public sf::Transformable {
    private:
        // Different colliders
        std::vector<Collider> colliders;

    public:
        // Body properties
        bool enabled;
        bool mStatic;

        // Constructors
        CollisionBody(float x, float y, float rotation);
        ~CollisionBody();

        // Functions
        unsigned int getCollidersSize();
        Collider& getCollider(unsigned int id);
        void addCollider(Collider c);
        void removeCollider(unsigned int id);
        void clearColliders();
        sf::Vector2f getPointOnCollider(unsigned int collider, unsigned int point);
        
        sf::Vector2f getUp();
        sf::Vector2f getRight();
    };

    class RigidBody : public CollisionBody {
    public:
        // Rigidbody properties
        sf::Vector2f velocity;
        sf::Vector2f acceleration;
        float rotVelocity;
        float rotAcceleration;
        float mass;

        // Constructors
        RigidBody(float x, float y, float rotation);
        ~RigidBody();

        // Static functions
        static RigidBody* createBoxRigidBody(float x, float y, float w, float h, float rotation = 0.f);
    };

    struct CollisionData {
        CollisionBody* c1;
        CollisionBody* c2;
        sf::Vector2f normal;
        float displacement;
        unsigned int contactPoint;
    };

};