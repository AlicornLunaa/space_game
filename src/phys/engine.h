/**
 * @file collider.h
 * @author Luna
 * @brief A collider contains the data for the collision bounds
 */
#pragma once
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "colliders.h"
#include "collisionbody.h"
#include "../interface/renderer.h"
#include "../util/vec_math.h"

namespace Phys {

    class Engine : public sf::Drawable {
    private:
        // Variables
        std::vector<CollisionBody*> bodies;
        std::vector<CollisionData> collisions;
        bool renderDebug; // Displays boxes and vectors

        // Functions
        bool collidesAABB(CollisionBody* c1, CollisionBody* c2);
        bool collidesSAT(CollisionBody* body1, CollisionBody* body2, int id1, int id2);

        void collisionDetection();
        void collisionResolution();
        void physicsUpdate();

    public:
        Engine();
        ~Engine();

        /**
         * @brief Adds a collider to the engine to get updated
         * @param c The collider to add
         * @return unsigned int The ID of the collider
         */
        unsigned int registerBody(CollisionBody* c);

        /**
         * @brief Removes the collider from the engine
         * @param id The collider to remove
         * @return true Removed successfully
         * @return false Collider does not exist
         */
        bool unregisterBody(unsigned int id);

        /**
         * @brief Get the Collider object
         * @param id 
         * @return Collider* 
         */
        CollisionBody* getBody(unsigned int id);

        /**
         * @brief Get the RigidBody object
         * @param id 
         * @return RigidBody* 
         */
        RigidBody* getRigidBody(unsigned int id);

        /**
         * @brief Update the physics objects
         * @param deltaTime Time between each frame
         */
        void update(float deltaTime);

        /**
         * @brief Renders the debug information
         * @param target
         * @param states 
         */
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    };

};