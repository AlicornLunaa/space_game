/**
 * @file player.h
 * @author Luna
 * @brief Holds the data and controls the keyboard and inputs
 */
#pragma once
#include <cmath>
#include <memory>
#include <SFML/Graphics.hpp>
#include "../phys/engine.h"
#include "vehicle.h"

namespace Objects {
    
    class Player : public sf::RectangleShape {
    private:
        // Variables
        sf::Texture tex;
        Phys::RigidBody* rigidbody;

        bool driving;
        Vehicles::Vehicle* vehicle;

        // Private functions
        void updateKeys(float deltaTime);

    public:
        Player(Phys::Engine& engine, float x, float y);
        Player();
        ~Player();

        /**
         * @brief Creates the object
         */
        void create(Phys::Engine& engine, float x, float y);

        // Vehicle functions
        /**
         * @brief Sets the entity being driven by the player
         * @param v The vehicle to drive
         */
        void drive(Vehicles::Vehicle* v);

        /**
         * @brief Gets the status if the player is driving or not
         * @return true The player is driving
         * @return false The player is not driving
         */
        bool isDriving();

        /**
         * @brief Get the Vehicle object
         * @return Vehicles::Vehicle* The vehicle being driven
         */
        Vehicles::Vehicle* getVehicle();

        /**
         * @brief Get the Rigid Body object
         * @return Phys::RigidBody* 
         */
        Phys::RigidBody* getRigidBody();

        // Physics functions
        /**
         * @brief Updates the player
         * @param deltaTime Difference between frames
         */
        void update(float deltaTime);

        // Rendering functions
        /**
         * @brief Draws all the extras of the player to the screen
         * @param target The target to render to
         */
        void render(sf::RenderTarget* target);
    };

};