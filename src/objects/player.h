/**
 * @file player.h
 * @author Luna
 * @brief Holds the data and controls the keyboard and inputs
 */
#pragma once
#include <SFML/Graphics.hpp>
#include "vehicle.h"

namespace Objects {
    
    class Player : public sf::RectangleShape {
    private:
        Vehicles::Vehicle* vehicle;

    public:
        Player(float x, float y);
        ~Player();

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