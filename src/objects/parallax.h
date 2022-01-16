/**
 * @file parallax.h
 * @author Luna
 * @brief Parallax class will draw stars to the background to show movement
 */
#pragma once
#include <SFML/Graphics.hpp>

namespace Objects {

    class Parallax : public sf::RectangleShape {
    private:
        sf::Texture tex;
        sf::Vector2f pos;
        float scale;
        
    public:
        Parallax(unsigned int width, unsigned int height, float scale);
        Parallax();
        ~Parallax();

        /**
         * @brief Creates the object
         * 
         * @param width 
         * @param height 
         * @param scale 
         */
        void create(unsigned int width, unsigned int height, float scale);

        /**
         * @brief Set the Camera Position object
         * @param p 
         */
        void setCameraPosition(sf::Vector2f p);

        /**
         * @brief Set the Scale object
         * @param s 
         */
        void setScale(float s);
    };

};