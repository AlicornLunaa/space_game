/**
 * @file ship.h
 * @author Luna
 * @brief Contains the information for physics collisions
 */
#pragma once
#include <memory>
#include <SFML/Graphics.hpp>

namespace Objects {

    class Ship : public sf::RectangleShape {
    private:
        sf::Texture texture;
        sf::Image body; // Image will contain the pixel data used to represent a fully modifiable ship

    public:
        /**
         * @brief Construct a new Ship object
         * @param width Width of the ship in pixels
         * @param height Height of the ship in pixels
         * @param scale The scale for the pixels
         */
        Ship(unsigned int width, unsigned int height, unsigned int scale = 4u);
        ~Ship();

        /**
         * @brief Resets the ship's texture to the new pixel data
         */
        void updateTexture();

        /**
         * @brief Updates the ship's physical characteristics
         * @param deltaTime Time between each frame
         */
        void update(float deltaTime);
    };

};