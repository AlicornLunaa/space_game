/**
 * @file ship.h
 * @author Luna
 * @brief Contains the information for physics collisions
 */
#pragma once
#include <memory>
#include <SFML/Graphics.hpp>
#include "../phys/engine.h"

namespace Vehicles {

    class Vehicle : public sf::RectangleShape {
    protected:
        sf::Texture tex;
        sf::Image body; // Image will contain the pixel data used to represent a fully modifiable ship
        
        Phys::BoxRigidBody* rigidbody;

    public:
        Vehicle(Phys::Engine& engine, float x, float y, unsigned int width, unsigned int height, unsigned int scale = 4u);
        ~Vehicle();

        /**
         * @brief Resets the ship's texture to the new pixel data
         */
        void updateTexture();

        /**
         * @brief Updates the physics by the keys
         * @param deltaTime 
         */
        virtual void updateKeys(float deltaTime) = 0;

        /**
         * @brief Updates the ship's physical characteristics
         * @param deltaTime Time between each frame
         */
        virtual void update(float deltaTime) = 0;
    };


    class Ship : public Vehicle {
    private:


    public:
        /**
         * @brief Construct a new Ship object
         * @param width Width of the ship in pixels
         * @param height Height of the ship in pixels
         * @param scale The scale for the pixels
         */
        Ship(Phys::Engine& engine, float x, float y);
        ~Ship();

        /**
         * @brief Updates the controls
         * @param deltaTime 
         */
        void updateKeys(float deltaTime);

        /**
         * @brief Updates the ship's physical characteristics
         * @param deltaTime Time between each frame
         */
        void update(float deltaTime);
    };

};