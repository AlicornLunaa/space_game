/**
 * @file collider.h
 * @author Luna
 * @brief A collider contains the data for the collision bounds
 */
#include <vector>
#include <SFML/Graphics.hpp>
#include "collider.h"
#include "../util/debugger.h"

namespace Phys {

    class Engine : public sf::Drawable {
    private:
        // Variables
        std::vector<Collider*> colliders;
        std::vector<CollisionData> collisions;
        bool renderDebug; // Displays boxes and vectors

        // Functions
        bool collidesAABB(Collider* c1, Collider* c2);

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
        unsigned int registerCollider(Collider* c);

        /**
         * @brief Removes the collider from the engine
         * @param id The collider to remove
         * @return true Removed successfully
         * @return false Collider does not exist
         */
        bool unregisterCollider(unsigned int id);

        /**
         * @brief Get the Collider object
         * @param id 
         * @return Collider* 
         */
        Collider* getCollider(unsigned int id);

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