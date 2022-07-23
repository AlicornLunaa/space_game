/**
 * @file rigidbody.h
 * @author Luna
 * @brief A body to have physics calculations applied to
 */
#pragma once
#include <SFML/System.hpp>
#include "colliders.h"
#include "../util/vec_math.h"
#include "../interface/renderer.h"

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
        
        sf::Vector2f getCenter();
        sf::Vector2f getUp();
        sf::Vector2f getRight();
        
        bool contains(sf::Vector2f point);
    };

    class RigidBody : public CollisionBody {
    public:
        // Rigidbody properties
        sf::Vector2f velocity;
        sf::Vector2f force;
        float rotVelocity;
        float torque;
        float mass;
        float inertia;
        float elasticity;
        float staticFriction;
        float dynamicFriction;

        // Constructors
        RigidBody(float x, float y, float rotation);
        ~RigidBody();

        // Functions
        void applyImpulse(sf::Vector2f impulse);
        void applyImpulse(sf::Vector2f impulse, sf::Vector2f contactVec);

        // Static functions
        static RigidBody* createBoxRigidBody(float x, float y, float w, float h, float rotation = 0.f);
    };

    struct CollisionData {
        CollisionBody* c1;
        CollisionBody* c2;
        sf::Vector2f normal;
        float displacement;
        sf::Vector2f contactPoint;
    };

};